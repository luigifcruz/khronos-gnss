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
                fs_frequency: trigger("fs_frequency", state.state.fs_frequency, action)
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
