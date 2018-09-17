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
        if (strstr(key, "ws_update_rate") || (key && !key[0])) {
            ApiHandler::AddKey((char*)"ws_update_rate", (char*)"settings", (char*)std::to_string(db->GetSettings().ws_update_rate).c_str(), changes);
        }
        if (strstr(key, "led_status") || (key && !key[0])) {
            ApiHandler::AddKey((char*)"led_status", (char*)"settings", (char*)std::to_string(db->GetSettings().led_status).c_str(), changes);
        }
        if (strstr(key, "serial_tx_active") || (key && !key[0])) {
            ApiHandler::AddKey((char*)"serial_tx_active", (char*)"settings", (char*)std::to_string(db->GetSettings().serial_tx_active).c_str(), changes);
        }
    }
    
    if (strstr(zone, "state") || (zone && !zone[0])) {
        if (strstr(key, "gps_fix_quality") || (key && !key[0])) {
            ApiHandler::AddKey((char*)"gps_fix_quality", (char*)"state", (char*)std::to_string(db->GetState().gps_fix_quality).c_str(), changes);
        }
        if (strstr(key, "gps_fix_type") || (key && !key[0])) {
            ApiHandler::AddKey((char*)"gps_fix_type", (char*)"state", (char*)std::to_string(db->GetState().gps_fix_type).c_str(), changes);
        }
        if (strstr(key, "gps_sat_numb") || (key && !key[0])) {
            ApiHandler::AddKey((char*)"gps_sat_numb", (char*)"state", (char*)std::to_string(db->GetState().gps_sat_numb).c_str(), changes);
        }
        if (strstr(key, "glonass_sat_numb") || (key && !key[0])) {
            ApiHandler::AddKey((char*)"glonass_sat_numb", (char*)"state", (char*)std::to_string(db->GetState().glonass_sat_numb).c_str(), changes);
        }
    }
    
    return cJSON_Print(res);
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