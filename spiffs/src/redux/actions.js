import React, { PropTypes } from 'react'

export const UPDATE_STATE = "UPDATE_STATE"
export const UPDATE_SETTINGS = "UPDATE_SETTINGS"

export const props = {
    'state': {
        'gnss_fix_quality': PropTypes.number,
        'gnss_fix_type': PropTypes.number,
        'sat_count_gps': PropTypes.number,
        'sat_count_glonass': PropTypes.number,
        'longitude': PropTypes.number,
        'latitude': PropTypes.number,
        'true_north': PropTypes.number,
        'ground_speed': PropTypes.number,
        'altitude': PropTypes.number
    },
    'settings': {
        'ws_update_rate': PropTypes.number,
        'led_status': PropTypes.number,
        'serial_tx_active': PropTypes.number
    }
}

export const mapStateToProps = (state) => ({
    'state': {
        'gnss_fix_quality': state.state.gnss_fix_quality,
        'gnss_fix_type': state.state.gnss_fix_type,
        'sat_count_gps': state.state.sat_count_gps,
        'sat_count_glonass': state.state.sat_count_glonass,
        'longitude': state.state.longitude,
        'latitude': state.state.latitude,
        'true_north': state.state.true_north,
        'ground_speed': state.state.ground_speed,
        'altitude': state.state.altitude
    },
    'settings': {
        'led_status': state.settings.led_status,
        'serial_tx_active': state.settings.serial_tx_active
    }
});

export function updateState(key, value) {
    return { type: UPDATE_STATE, key, value }
}

export function updateSettings(key, value) {
    return { type: UPDATE_SETTINGS, key, value }
}