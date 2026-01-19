/*
 * @Date: 2026-01-12 14:40:30
 * @LastEditors: GiddyPoet
 * @LastEditTime: 2026-01-19 10:37:56
 */
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <time.h>

#define UNUSED __attribute__((unused))

UNUSED static ngx_int_t ngx_http_hello_tmp_buff_handler(ngx_http_request_t * r) {
    ngx_buf_t * b;
    ngx_chain_t out;
    ngx_int_t rc = 0;

    if (!(r->method & NGX_HTTP_GET)) {
        return NGX_HTTP_NOT_ALLOWED;
    }

    rc = ngx_http_discard_request_body(r);
    if (rc != NGX_OK) {
        return rc;
    }

    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_type.data = (u_char *)"text/plain";
    r->headers_out.content_type.len = sizeof("text/plain") - 1;

    b = ngx_create_temp_buf(r->pool, 128);
    if (b == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    /* ngx_snprintf 返回的是写完后的最后一个地址 */
    b->last = ngx_snprintf(b->pos, b->end - b->pos, "Hello, World! Current server time is: %s", ctime(&(time_t){time(NULL)}));
    b->last_buf = 1;


    out.buf = b;
    out.next = NULL;

    ngx_http_send_header(r);

    /* 发送响应体 */
    return ngx_http_output_filter(r, &out);
}

static ngx_int_t ngx_http_hello_send_file_handler(ngx_http_request_t * r) {
    ngx_buf_t * b = NULL;
    ngx_chain_t out;
    ngx_int_t rc = 0;

    if (!(r->method & NGX_HTTP_GET)) {
        return NGX_HTTP_NOT_ALLOWED;
    }
    rc = ngx_http_discard_request_body(r);
    if (rc != NGX_OK) {
        return rc;
    }

    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_type.data = (u_char *)"text/plain";
    r->headers_out.content_type.len = sizeof("text/plain") - 1;

    b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
    if (b == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    ngx_str_t filename = ngx_string("/tmp/hello.txt");
    b->file = ngx_pcalloc(r->pool, sizeof(ngx_file_t));
    if (b->file == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    /* 需要借助于ngx_pool_cleanup清除文件句柄 */
    b->file->fd = ngx_open_file(filename.data, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,  NGX_FILE_OPEN, 0);
    if (b->file->fd == NGX_INVALID_FILE) {
        return NGX_HTTP_NOT_FOUND;
    }


    /* register cleanup worker */
    ngx_pool_cleanup_t * cln = ngx_pool_cleanup_add(r->pool, sizeof(ngx_pool_cleanup_t));
    if (cln == NULL) {
        return NGX_ERROR;
    }
    cln->handler = ngx_pool_cleanup_file;

    ngx_pool_cleanup_file_t * clnf = cln->data;
    clnf->fd = b->file->fd;
    clnf->name = b->file->name.data;
    clnf->log = r->pool->log;

    b->file->name = filename;
    b->file->log = r->connection->log;
    if (ngx_file_info(filename.data, &b->file->info) == NGX_FILE_ERROR) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    r->headers_out.content_length_n = b->file->info.st_size;
    b->file_pos = 0;
    b->file_last = b->file->info.st_size;
    b->in_file = 1;
    b->last_buf = 1;


    out.buf = b;
    out.next = NULL;

    ngx_http_send_header(r);

    return ngx_http_output_filter(r, &out);
}



__attribute__((unused)) static ngx_int_t ngx_http_hello_handler(ngx_http_request_t * r) {
    ngx_buf_t * b;
    ngx_chain_t out;
    ngx_int_t rc = 0;

    if (!(r->method & NGX_HTTP_GET)) {
        return NGX_HTTP_NOT_ALLOWED;
    }

    rc = ngx_http_discard_request_body(r);
    if (rc != NGX_OK) {
        return rc;
    }

    /* 设置响应头部 */
    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_type.len = sizeof("text/plain") - 1;
    r->headers_out.content_type.data = (u_char *)"text/plain";

    /* 分配响应体内存 */
    b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
    if (b == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    /* 准备响应体内容 */
    time_t now = time(NULL);
    u_char * response = ngx_pcalloc(r->pool, 64);
    ngx_snprintf(response, 64, "Hello, World! Current server time is: %s", ctime(&now));

    b->pos = response; /* 指向响应体开始位置 */
    b->last = response + ngx_strlen(response); /* 指向响应体结束位置 */
    b->memory = 1; /* 内容在内存中 */
    b->last_buf = 1; /* 最后一个缓冲区 */

    out.buf = b;
    out.next = NULL;

    r->headers_out.content_length_n = b->last - b->pos;

    /* 发送响应头 */
    ngx_http_send_header(r);

    /* 发送响应体 */
    return ngx_http_output_filter(r, &out);
}


// typedef ngx_int_t (*ngx_http_handler_pt)(ngx_http_request_t *r);
/* handle config */
static char * ngx_http_hello_set(ngx_conf_t * cf, ngx_command_t * cmd, void * conf) {
    ngx_http_core_loc_conf_t* clconf = NULL;

    clconf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);

    /* TODO: INIT handler */
    // clconf->handler = ngx_http_hello_handler;
    clconf->handler = ngx_http_hello_send_file_handler;

    return NGX_CONF_OK;
}

static ngx_command_t ngx_http_hello_commands [] = {
    { 
        ngx_string("hello"), 
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LMT_CONF | NGX_CONF_NOARGS,
        ngx_http_hello_set,
        NGX_HTTP_LOC_CONF_OFFSET,
        0,
        NULL
    },

    ngx_null_command,
};


static ngx_http_module_t ngx_http_hello_module_ctx = {
    NULL,                          /* preconfiguration */
    NULL,                          /* postconfiguration */

    NULL,                          /* create main configuration */
    NULL,                          /* init main configuration */

    NULL,                          /* create server configuration */
    NULL,                          /* merge server configuration */

    NULL,                          /* create location configuration */
    NULL                           /* merge location configuration */
};

ngx_module_t ngx_http_hello_module = {
    NGX_MODULE_V1,
    &ngx_http_hello_module_ctx,    /* module context */
    ngx_http_hello_commands,
    NGX_HTTP_MODULE,               /* module type */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NGX_MODULE_V1_PADDING
};

ngx_module_t ngx_http_hello_module;

/* ngx_module_t -> ngx_http_module_t -> set -> */