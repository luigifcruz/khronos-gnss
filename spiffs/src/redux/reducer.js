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
                gnss_fix_quality: trigger("gnss_fix_quality", state.state.gnss_fix_quality, action),
                gnss_fix_type: trigger("gnss_fix_type", state.state.gnss_fix_type, action),
                sat_count_gps: trigger("sat_count_gps", state.state.sat_count_gps, action),
                sat_count_glonass: trigger("sat_count_glonass", state.state.sat_count_glonass, action),
                longitude: trigger("longitude", state.state.longitude, action),
                latitude: trigger("latitude", state.state.latitude, action),
                true_north: trigger("true_north", state.state.true_north, action),
                ground_speed: trigger("ground_speed", state.state.ground_speed, action),
                altitude: trigger("altitude", state.state.altitude, action)
            })
        });

        case UPDATE_SETTINGS:
        return Object.assign({}, state, {
            settings: Object.assign({}, state.settings, {
                led_status: trigger("led_status", state.settings.led_status, action),
                serial_tx_active: trigger("serial_tx_active", state.settings.serial_tx_active, action)
            })
        });

        default:
        return state;
    }
}