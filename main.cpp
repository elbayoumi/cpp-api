#include <microhttpd.h>
#include <cstring>
#include <iostream>
using namespace std;
#define PORT 8080

static MHD_Result request_handler(void *cls, struct MHD_Connection *connection, const char *url,
                                  const char *method, const char *version, const char *upload_data,
                                  size_t *upload_data_size, void **con_cls) {
    if (strcmp(method, "GET") == 0 || strcmp(method, "POST") == 0) {
        // Extract parameter from the URI path
        const char *last_slash = strrchr(url, '/');
        const char *param = (last_slash != nullptr) ? last_slash + 1 : url;

        string response_text = "Request received with parameter:\n";
        response_text += "Parameter: " + string(param) + "\n";

        struct MHD_Response *response = MHD_create_response_from_buffer(response_text.length(),
                                                                       (void *) response_text.c_str(),
                                                                       MHD_RESPMEM_MUST_COPY);
        int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        return ret == MHD_YES ? MHD_YES : MHD_NO;
    } else {
        return MHD_NO;  // Unsupported HTTP method
    }
}

int main() {
    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(MHD_USE_AUTO | MHD_USE_INTERNAL_POLLING_THREAD, PORT, nullptr, nullptr,
                              &request_handler, nullptr, MHD_OPTION_END);

    if (daemon == nullptr) {
        cerr << "Error starting MHD daemon." << endl;
        return 1;
    }

    cout << "Server started on port " << PORT << endl;
    cout << "Press Enter to exit..." << endl;
    cin.get();

    MHD_stop_daemon(daemon);
    return 0;
}
