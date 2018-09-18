#include "api_handler.h"

void ApiHandler::AddKey(char* key, char* zone, char* value, cJSON* dest) {
    cJSON *key_changes = cJSON_CreateObject();

    cJSON_AddStringToObject(key_changes, "key", key);
    cJSON_AddStringToObject(key_changes, "zone", zone);
    cJSON_AddStringToObject(key_changes, "value", value);

    cJSON_AddItemToArray(dest, key_changes);
}

char* ApiHandler::Response(char* key, char* zone, Database* db) {
    cJSON *res, *changes;

	res = cJSON_CreateObject();	
	cJSON_AddItemToObject(res, "type", cJSON_CreateString("broadcast"));

    if ((zone && !zone[0]) || (key && !key[0])) {
        cJSON_AddItemToObject(res, "method", cJSON_CreateString("bulk_update"));
    } else {
        cJSON_AddItemToObject(res, "method", cJSON_CreateString("delta_update"));
    }
    
    changes = cJSON_AddArrayToObject(res, "changes");

    if (strstr(zone, "settings") || (zone && !zone[0])) {
        if (strstr(key, "led_status ") || (key && !key[0])) {
            ApiHandler::AddKey((char*)"led_status", (char*)"settings", (char*)std::to_string(db->GetSettings().led_status).c_str(), changes);
        }
        if (strstr(key, "serial_tx_active ") || (key && !key[0])) {
            ApiHandler::AddKey((char*)"serial_tx_active", (char*)"settings", (char*)std::to_string(db->GetSettings().serial_tx_active).c_str(), changes);
        }
    }
    
    if (strstr(zone, "state") || (zone && !zone[0])) {
        if (strstr(key, "gnss_fix_quality ") || (key && !key[0])) {
            ApiHandler::AddKey((char*)"gnss_fix_quality", (char*)"state", (char*)std::to_string(db->GetState().gnss_fix_quality).c_str(), changes);
        }
        if (strstr(key, "gnss_fix_type ") || (key && !key[0])) {
            ApiHandler::AddKey((char*)"gnss_fix_type", (char*)"state", (char*)std::to_string(db->GetState().gnss_fix_type).c_str(), changes);
        }
        if (strstr(key, "sat_count_gps ") || (key && !key[0])) {
            ApiHandler::AddKey((char*)"sat_count_gps", (char*)"state", (char*)std::to_string(db->GetState().sat_count_gps).c_str(), changes);
        }
        if (strstr(key, "sat_count_glonass ") || (key && !key[0])) {
            ApiHandler::AddKey((char*)"sat_count_glonass", (char*)"state", (char*)std::to_string(db->GetState().sat_count_glonass).c_str(), changes);
        }
        if (strstr(key, "latitude ") || (key && !key[0])) {
            ApiHandler::AddKey((char*)"latitude", (char*)"state", (char*)std::to_string(db->GetState().latitude).c_str(), changes);
        }
        if (strstr(key, "longitude ") || (key && !key[0])) {
            ApiHandler::AddKey((char*)"longitude", (char*)"state", (char*)std::to_string(db->GetState().longitude).c_str(), changes);
        }
        if (strstr(key, "true_north ") || (key && !key[0])) {
            ApiHandler::AddKey((char*)"true_north", (char*)"state", (char*)std::to_string(db->GetState().true_north).c_str(), changes);
        }
        if (strstr(key, "ground_speed ") || (key && !key[0])) {
            ApiHandler::AddKey((char*)"ground_speed", (char*)"state", (char*)std::to_string(db->GetState().ground_speed).c_str(), changes);
        }
        if (strstr(key, "altitude ") || (key && !key[0])) {
            ApiHandler::AddKey((char*)"altitude", (char*)"state", (char*)std::to_string(db->GetState().altitude).c_str(), changes);
        }
    }
    
    char* payload = cJSON_Print(res);
    cJSON_Delete(res);
    return payload;
}

char* ApiHandler::Request(cJSON* req, Database* db, bool* broadcast) {

    if (req != NULL) {
        char* method = cJSON_GetObjectItemCaseSensitive(req, "method")->valuestring;
        char* target = cJSON_GetObjectItemCaseSensitive(req, "target")->valuestring;

        if (strstr(method, "get")) {
            if (strstr(target, "data")) {
                *broadcast = true;
            }
        }

        if (strstr(method, "set")) {

            if (strstr(target, "led_on")) {
                Settings s = db->GetSettings();
                s.led_status = 0x0001;
                db->UpdateSettings(s);
            }

            if (strstr(target, "led_off")) {
                Settings s = db->GetSettings();
                s.led_status = 0x0000;
                db->UpdateSettings(s);
            }

            if (strstr(target, "serial_tx_on")) {
                Settings s = db->GetSettings();
                s.serial_tx_active = 0x01;
                db->UpdateSettings(s);
            }

            if (strstr(target, "serial_tx_off")) {
                Settings s = db->GetSettings();
                s.serial_tx_active = 0x00;
                db->UpdateSettings(s);
            }
        }
    }

    return Response((char*)"", (char*)"", db);
}