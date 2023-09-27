#include <microhttpd.h>
#include <cstring>
#include <iostream>

#define PORT 8080

static MHD_Result request_handler(void *cls, struct MHD_Connection *connection, const char *url,
                                  const char *method, const char *version, const char *upload_data,
                                  size_t *upload_data_size, void **con_cls) {
    const char *page = "<html><body>Hello, World!</body></html>";
    struct MHD_Response *response;
    int ret;

    if (strcmp(method, "GET") == 0) {
        response = MHD_create_response_from_buffer(strlen(page), (void *) page, MHD_RESPMEM_PERSISTENT);
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        return MHD_YES;
    } else if (strcmp(method, "POST") == 0) {
        const char *post_data = upload_data;
        if (*upload_data_size > 0) {
            // Process the POST data (in 'post_data') here
        }

        const char *post_response = "POST request received.";
        response = MHD_create_response_from_buffer(strlen(post_response), (void *) post_response, MHD_RESPMEM_PERSISTENT);
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        return MHD_YES;
    } else {
        return MHD_NO;  // Unsupported HTTP method
    }
}

int main() {
    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(MHD_USE_AUTO | MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL, &request_handler, NULL, MHD_OPTION_END);

    if (daemon == nullptr) {
        std::cerr << "Error starting MHD daemon." << std::endl;
        return 1;
    }

    std::cout << "Server started on port " << PORT << std::endl;
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    MHD_stop_daemon(daemon);
    return 0;
}
