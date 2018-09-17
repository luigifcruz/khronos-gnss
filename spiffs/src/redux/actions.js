import React, { PropTypes } from 'react'

export const UPDATE_STATE = "UPDATE_STATE"
export const UPDATE_SETTINGS = "UPDATE_SETTINGS"

export const props = {
    'state': {
        'gps_fix_quality': PropTypes.number,
        'gps_fix_type': PropTypes.number,
        'gps_sat_numb': PropTypes.number,
        'glonass_sat_numb': PropTypes.number
    },
    'settings': {
        'ws_update_rate': PropTypes.number,
        'led_status': PropTypes.number,
        'serial_tx_active': PropTypes.number
    }
}

export const mapStateToProps = (state) => ({
    'state': {
        'gps_fix_quality': state.state.gps_fix_quality,
        'gps_fix_type': state.state.gps_fix_type,
        'gps_sat_numb': state.state.gps_sat_numb,
        'glonass_sat_numb': state.state.glonass_sat_numb
    },
    'settings': {
        'ws_update_rate': state.settings.ws_update_rate,
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