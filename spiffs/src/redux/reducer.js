import {
    UPDATE_STATE,
    UPDATE_SETTINGS
} from "./actions"

function trigger(key, value, action) {
    return (action.key == key) ? action.value : value;
}

export default function reducer(state, action) {
    switch (action.type) {
        case UPDATE_STATE:
        return Object.assign({}, state, {
            state: Object.assign({}, state.state, {
                gps_fix_quality: trigger("gps_fix_quality", state.state.gps_fix_quality, action),
                gps_fix_type: trigger("gps_fix_type", state.state.gps_fix_type, action),
                gps_sat_numb: trigger("gps_sat_numb", state.state.gps_sat_numb, action),
                glonass_sat_numb: trigger("glonass_sat_numb", state.state.glonass_sat_numb, action)
            })
        });

        case UPDATE_SETTINGS:
        return Object.assign({}, state, {
            settings: Object.assign({}, state.settings, {
                ws_update_rate: trigger("ws_update_rate", state.settings.ws_update_rate, action),
                led_status: trigger("led_status", state.settings.led_status, action),
                serial_tx_active: trigger("serial_tx_active", state.settings.serial_tx_active, action)
            })
        });

        default:
        return state;
    }
}
