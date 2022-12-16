#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <microhttpd.h>

#define PORT 8888

int print_headers(void *cls,
                  enum MHD_ValueKind kind,
                  const char *key,
                  const char *value)
{
    printf("%s: %s\n", key, value);
    return MHD_YES;
}

// Callback function that will be called when a request is received
int handle_request(void *cls,
                   struct MHD_Connection *connection,
                   const char *url,
                   const char *method,
                   const char *version,
                   const char *upload_data,
                   size_t *upload_data_size,
                   void **con_cls)
{
    if (*con_cls == NULL) {
        // Allocate memory for the request body buffer
        char *request_body = malloc(1024);

        // Store the request body buffer in the con_cls pointer
        *con_cls = request_body;

        // Copy the request body to the buffer
        memcpy(request_body, upload_data, *upload_data_size);

        // Set the upload_data_size to the size of the request body buffer
        *upload_data_size = 1024;

        // Return MHD_YES to indicate that the callback should be called again
        return MHD_YES;
    } else {
        char *request_body = *con_cls;

        // Print the request body
        printf("Request body: %s\n", request_body);

    // Print the request headers
    const char *header_name;
    const char *header_value;
    printf("Request headers:\n");
    MHD_get_connection_values(connection, MHD_HEADER_KIND,
        &print_headers, NULL);


    // Send a response to the client
    const char *response_str = "Hello, World!";
    struct MHD_Response *response;
    int ret;

    response = MHD_create_response_from_buffer(strlen(response_str),
                                               (void *)response_str,
                                               MHD_RESPMEM_MUST_COPY);
    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    free(request_body)
    return ret;
    }
}



int main()
{
    struct MHD_Daemon *daemon;

    // Create a HTTP server that listens on port 8888
    daemon = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION,
                              PORT,
                              NULL,
                              NULL,
                              &handle_request,
                              NULL,
                              MHD_OPTION_END);
    if (daemon == NULL) {
        printf("Error starting HTTP server.\n");
        return 1;
    }

    // Wait for the user to press enter before exiting
    getchar();

    // Stop the HTTP server
    MHD_stop_daemon(daemon);

    return 0;
}
