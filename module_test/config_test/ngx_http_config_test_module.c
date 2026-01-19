/*
 * @Date: 2026-01-19 11:28:38
 * @LastEditors: GiddyPoet
 * @LastEditTime: 2026-01-19 17:39:08
 */
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

ngx_module_t ngx_http_config_test_module;

typedef struct {
    ngx_str_t  my_str;
    ngx_int_t  my_num;
    ngx_flag_t my_flag;
    size_t my_size;
    ngx_array_t *my_array;
    ngx_array_t *my_keyval;
    off_t my_off;
    time_t my_sec;
    ngx_msec_t my_msec;
    ngx_bufs_t my_bufs;
    ngx_uint_t my_enum_seq;
    ngx_uint_t my_bitmask;
    ngx_uint_t my_access;
    ngx_path_t *my_path;
}ngx_http_config_test_conf_t;


typedef struct {
    ngx_str_t test_config_str;
    ngx_int_t test_config_num;
}ngx_http_config_self_conf_t;

static char * ngx_conf_set_str_num_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_http_config_self_conf_t  *self_conf = conf;

    ngx_str_t        *value;

    value = cf->args->elts;


    self_conf->test_config_str = value[1];
    self_conf->test_config_num = ngx_atoi(value[2].data, value[2].len);

    return NGX_CONF_OK;
}

static ngx_int_t ngx_http_config_test_handler(ngx_http_request_t *r) {
    ngx_http_config_test_conf_t  *conf;

    conf = ngx_http_get_module_loc_conf(r, ngx_http_config_test_module);

    ngx_str_t response = conf->my_str;

    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = response.len;
    r->headers_out.content_type.len = sizeof("text/plain") - 1;
    r->headers_out.content_type.data = (u_char *)"text/plain";

    ngx_http_send_header(r);

    ngx_buf_t    *b;
    ngx_chain_t   out;

    b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
    out.buf = b;
    out.next = NULL;

    b->pos = response.data;
    b->last = response.data + response.len;
    b->memory = 1;
    b->last_buf = 1;

    return ngx_http_output_filter(r, &out);
}

static char *ngx_http_core_dump_config(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_http_core_loc_conf_t  *clcf;

    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_config_test_handler;

    return NGX_OK;
}


/* ngx_module_t -> ngx_http_module_t -> ngx_command_t */

static ngx_command_t ngx_http_config_test_commands[] = {
        {
                ngx_string("my_str"),
                NGX_HTTP_LOC_CONF | NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_CONF_TAKE1,
                ngx_conf_set_str_slot,
                NGX_HTTP_LOC_CONF_OFFSET,
                offsetof(ngx_http_config_test_conf_t, my_str),
                NULL,
        },
        {
                ngx_string("my_num"),
                NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
                ngx_conf_set_num_slot,
                NGX_HTTP_LOC_CONF_OFFSET,
                offsetof(ngx_http_config_test_conf_t, my_num),
                NULL,
        },
        {
                ngx_string("my_flag"),
                NGX_HTTP_LOC_CONF | NGX_CONF_FLAG,
                ngx_conf_set_flag_slot,
                NGX_HTTP_LOC_CONF_OFFSET,
                offsetof(ngx_http_config_test_conf_t, my_flag),
                NULL,
        },
        {
            ngx_string("my_size"),
            NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
            ngx_conf_set_size_slot,
            NGX_HTTP_LOC_CONF_OFFSET,
            offsetof(ngx_http_config_test_conf_t, my_size),
            NULL,
        },
        {
            ngx_string("my_array"),
            NGX_HTTP_LOC_CONF | NGX_CONF_1MORE,
            ngx_conf_set_str_array_slot,
            NGX_HTTP_LOC_CONF_OFFSET,
            offsetof(ngx_http_config_test_conf_t, my_array),
            NULL,
        },
        {
            ngx_string("my_keyval"),
            NGX_HTTP_LOC_CONF | NGX_CONF_1MORE,
            ngx_conf_set_keyval_slot,
            NGX_HTTP_LOC_CONF_OFFSET,
            offsetof(ngx_http_config_test_conf_t, my_keyval),
            NULL,
        },
        {
            ngx_string("my_msec"),
            NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
            ngx_conf_set_msec_slot,
            NGX_HTTP_LOC_CONF_OFFSET,
            offsetof(ngx_http_config_test_conf_t, my_msec),
            NULL,
        },
        {
            ngx_string("my_sec"),
            NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
            ngx_conf_set_sec_slot,
            NGX_HTTP_LOC_CONF_OFFSET,
            offsetof(ngx_http_config_test_conf_t, my_sec),
            NULL,
        },
        {
            ngx_string("my_off"),
            NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
            ngx_conf_set_off_slot,
            NGX_HTTP_LOC_CONF_OFFSET,
            offsetof(ngx_http_config_test_conf_t, my_off),
            NULL,
        },
        {
            ngx_string("my_bufs"),
            NGX_HTTP_LOC_CONF | NGX_CONF_TAKE2,
            ngx_conf_set_bufs_slot,
            NGX_HTTP_LOC_CONF_OFFSET,
            offsetof(ngx_http_config_test_conf_t, my_bufs),
            NULL,
        },
        {
            ngx_string("my_enum_seq"),
            NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
            ngx_conf_set_enum_slot,
            NGX_HTTP_LOC_CONF_OFFSET,
            offsetof(ngx_http_config_test_conf_t, my_enum_seq),
            (ngx_conf_enum_t[]) {
                { ngx_string("one"), 1 },
                { ngx_string("two"), 2 },
                { ngx_string("three"), 3 },
                { ngx_null_string, 0 }
            },
        },
        {
            ngx_string("my_bitmask"),
            NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
            ngx_conf_set_bitmask_slot,
            NGX_HTTP_LOC_CONF_OFFSET,
            offsetof(ngx_http_config_test_conf_t, my_bitmask),
            (ngx_conf_bitmask_t[]) {
                { ngx_string("read"), 0x01 },
                { ngx_string("write"), 0x02 },
                { ngx_string("excute"), 0x04 },
                { ngx_null_string, 0 }
            },
        },
        {
            ngx_string("my_access"),
            NGX_HTTP_LOC_CONF | NGX_CONF_TAKE123,
            ngx_conf_set_access_slot,
            NGX_HTTP_LOC_CONF_OFFSET,
            offsetof(ngx_http_config_test_conf_t, my_access),
            NULL,
        },
        {
            ngx_string("my_path"),
            NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1234,
            ngx_conf_set_path_slot,
            NGX_HTTP_LOC_CONF_OFFSET,
            offsetof(ngx_http_config_test_conf_t, my_path),
            NULL,
        },
        {
            ngx_string("self_config"),
            NGX_HTTP_LOC_CONF | NGX_CONF_TAKE2,
            ngx_conf_set_str_num_slot,
            NGX_HTTP_LOC_CONF_OFFSET,
            0,
            NULL,
        },
        {
            ngx_string("config_test_dump"),
            NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS,
            ngx_http_core_dump_config,
            NGX_HTTP_LOC_CONF_OFFSET,
            0,
            NULL,
        },

        ngx_null_command};

static  void * create_http_loc_config_test_conf(ngx_conf_t *cf) {
    ngx_http_config_test_conf_t  *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_config_test_conf_t));

    if (conf == NULL) {
        return NULL;
    }

    conf->my_num = NGX_CONF_UNSET;
    conf->my_flag = NGX_CONF_UNSET;
    conf->my_size = NGX_CONF_UNSET_SIZE;
    conf->my_off = NGX_CONF_UNSET;
    conf->my_msec = NGX_CONF_UNSET_MSEC;
    conf->my_sec = NGX_CONF_UNSET;
    conf->my_enum_seq = NGX_CONF_UNSET_UINT;
    conf->my_bitmask = NGX_CONF_UNSET_UINT;
    conf->my_access = NGX_CONF_UNSET_UINT;
    conf->my_path = NGX_CONF_UNSET_PTR;
    conf->my_array = NGX_CONF_UNSET_PTR;
    conf->my_keyval = NGX_CONF_UNSET_PTR;
    conf->my_bufs.num = NGX_CONF_UNSET_SIZE;
    conf->my_bufs.size = NGX_CONF_UNSET_SIZE;
    conf->my_str.len = 0;
    conf->my_str.data = NULL;


    return conf;
}

static ngx_http_module_t ngx_http_config_test_module_ctx = {
    NULL,                          /* preconfiguration */
    NULL,                          /* postconfiguration */

    NULL,                          /* create main configuration */
    NULL,                          /* init main configuration */

    NULL,                          /* create server configuration */
    NULL,                          /* merge server configuration */

    create_http_loc_config_test_conf,                          /* create location configuration */
    NULL                           /* merge location configuration */
};


ngx_module_t ngx_http_config_test_module = {
    NGX_MODULE_V1,
    &ngx_http_config_test_module_ctx,                          /* module context */
    ngx_http_config_test_commands,                          /* module directives */
    NGX_HTTP_MODULE,               /* module type */
    NULL,                          /* init master */
    NULL,                          /* init module */
    NULL,                          /* init process */
    NULL,                          /* init thread */
    NULL,                          /* exit thread */
    NULL,                          /* exit process */
    NULL,                          /* exit master */
    NGX_MODULE_V1_PADDING
};


