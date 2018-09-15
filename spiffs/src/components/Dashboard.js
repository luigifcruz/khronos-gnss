import React, { Component, PropTypes } from 'react'
import { Link } from 'react-router'
import '../styles/Dashboard.scss'

class Dashboard extends Component {

    render() {
        return (
            <div className="Dashboard">
                <p>Welcome to Khronos!</p>
                <Link to="/gnss">GNSS Dashboard</Link>
                <Link to="/tuner">Frequency Tuner</Link>
                <Link to="/clock">NTP Clock</Link>
                <Link to="/settings">Settings</Link>
            </div>
        )
    }

}

export default Dashboard
