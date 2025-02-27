/////////////////////////////////////////////////////////////////////
// Project: Raspberry Pi Pico TLS Client Application
// Author: Kevin Thomas
// E-Mail: ket189@pitt.edu
// Version: 1.0
// Date: 02/26/25
// Target Device: Raspberry Pi Pico W (RP2040)
// Clock Frequency: (Depends on your board, e.g., 125 MHz)
// Toolchain: CMake, pico-sdk, ARM-none-eabi-gcc
// License: Apache License 2.0
// Description: This application initializes standard I/O and Wi‑Fi,
//              then runs the TLS client test which connects securely
//              to a specified server and sends an HTTP request.
//              It utilizes the common TLS client functions provided 
//              in tls_common.h/tls_common.c.
/////////////////////////////////////////////////////////////////////

 #include "pico/stdlib.h"
 #include "pico/cyw43_arch.h"
 #include "wifi_creds.h"
 #include <stdio.h>
 #include "tls_common.h"
 
 #define TLS_CLIENT_SERVER        "worldtimeapi.org"
 #define TLS_CLIENT_HTTP_REQUEST  "GET /api/ip HTTP/1.1\r\n" \
                                  "Host: " TLS_CLIENT_SERVER "\r\n" \
                                  "Connection: close\r\n" \
                                  "\r\n"
 #define TLS_CLIENT_TIMEOUT_SECS  15
 
 int main(void) {
     stdio_init_all();
 
     if (cyw43_arch_init()) {
         printf("failed to initialise\n");
         return 1;
     }
     cyw43_arch_enable_sta_mode();
 
     if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
         printf("failed to connect\n");
         return 1;
     }
     bool pass = run_tls_client_test(NULL, 0, TLS_CLIENT_SERVER, TLS_CLIENT_HTTP_REQUEST, TLS_CLIENT_TIMEOUT_SECS);
     if (pass) {
         printf("test passed\n");
     } else {
         printf("test failed\n");
     }
     /* Sleep a bit to allow USB stdio to flush buffers */
     sleep_ms(100);
 
     cyw43_arch_deinit();
     printf("all done...\n");
     return pass ? 0 : 1;
 }
