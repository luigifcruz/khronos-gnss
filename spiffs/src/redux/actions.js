import PropTypes from 'prop-types'
import React from 'react'

export const UPDATE_STATE = "UPDATE_STATE"
export const UPDATE_SETTINGS = "UPDATE_SETTINGS"

export const props = {
    'state': PropTypes.object.isRequired,
    'settings': PropTypes.object.isRequired
}

export const mapStateToProps = (state) => ({
    'state': state.state,
    'settings': state.settings
});

export function updateState(key, value) {
    return { type: UPDATE_STATE, key, value }
}

export function updateSettings(key, value) {
    return { type: UPDATE_SETTINGS, key, value }
}