#include <stdio.h>
#include <string.h>
#include "supabase_client.h"

static const char *TAG = "SUPABASE_CLIENT";

static char auth[250] = "Bearer ";
static esp_http_client_handle_t client;

void spb_open(supabase_config *config_info)
{
    supabase_config *config = (supabase_config*)config_info;
    esp_http_client_config_t esp_http_client_config = {
        .url = config->table_url,
        .method = HTTP_METHOD_POST,
        .event_handler = on_client_data,
        .buffer_size = 15000,              /*!< HTTP receive buffer size */
        .buffer_size_tx = 15000           /*!< HTTP transmit buffer size */
    };

    ESP_LOGI(TAG, "Inicializando Supabase (HTTP) Client");
    strcat(auth, config->api_key);
    client = esp_http_client_init(&esp_http_client_config);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_header(client, "Prefer", "return=representation");
    esp_http_client_set_header(client, "apikey", config->api_key);
    esp_http_client_set_header(client, "Authorization", auth);
}

void spb_init(void)
{
    ESP_LOGI(TAG, "Inicializando Supabase (HTTP) Client");
    // strcat(auth, config.api_key);
    // client = esp_http_client_init(&esp_http_client_config);
    // esp_http_client_set_header(client, "Content-Type", "application/json");
    // esp_http_client_set_header(client, "Prefer", "return=representation");
    // esp_http_client_set_header(client, "apikey", config.api_key);
    // esp_http_client_set_header(client, "Authorization", auth);
}

void spb_close(void)
{
    esp_http_client_cleanup(client);
}

void spb_read(void)
{
    ESP_LOGI(TAG, "Reading data from Supabase...");
}

void spb_write(void *data)
{
    char *payload_body = (char*) data;
    esp_http_client_set_post_field(client, payload_body, strlen(payload_body));
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Successfully POST sent. Status = %d, Content_length = %d",
                    esp_http_client_get_status_code(client),
                    esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "POST request failed: %s", esp_err_to_name(err));
    }
}

esp_err_t on_client_data(esp_http_client_event_t *evt)
{
    switch(evt->event_id) {
        // Quando chega dados
        case HTTP_EVENT_ON_DATA:
            // ESP_LOGI(TAG, "Length=%d", evt->data_len);
            // printf("%.*s\n", evt->data_len, (char *)evt->data);
            break;
        default:
            break;
    }

    return ESP_OK;
}

void fetch_data(void)
{
    esp_http_client_config_t esp_http_client_config = {
        .url = "",
        .method = HTTP_METHOD_GET,
        .event_handler = on_client_data,
        .buffer_size = 15000,              /*!< HTTP receive buffer size */
        // .buffer_size_tx = 10000           /*!< HTTP transmit buffer size */
    };

    esp_http_client_handle_t client = esp_http_client_init(&esp_http_client_config);
    esp_http_client_set_header(client, "apikey", "");
    esp_http_client_set_header(client, "Authorization", "Bearer ");
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK){
        ESP_LOGI("HTTP_Client", "HTTP GET status = %d, content_length = %d",
                    esp_http_client_get_status_code(client),
                    esp_http_client_get_content_length(client));
    }else{
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);
    // wifi_disconnect();
}
