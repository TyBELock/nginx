#!/bin/bash

# configure
./auto/configure --prefix=/usr/local/nginx --with-http_ssl_module --with-http_realip_module --with-http_gzip_static_module --with-http_stub_status_module --with-http_v2_module --with-http_v3_module --with-stream --with-stream_ssl_module --add-module=/root/code/nginx/module_test

# make
make -j4
