import React, { PropTypes } from 'react'

export const UPDATE_STATE = "UPDATE_STATE"
export const UPDATE_SETTINGS = "UPDATE_SETTINGS"

export const props = {
    'state': {
        'fs_frequency': PropTypes.number
    },
    'settings': {
        'ws_update_rate': PropTypes.number,
        'led_status': PropTypes.number,
        'serial_tx_active': PropTypes.number
    }
}

export const mapStateToProps = (state) => ({
    'state': {
        'fs_frequency': state.state.fs_frequency
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