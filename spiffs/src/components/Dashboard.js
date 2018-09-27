import React, { Component } from 'react'
import { Link } from 'react-router-dom'
import '../styles/Dashboard.scss'

class Dashboard extends Component {

    render() {
        return (
            <div className="Dashboard">
                <h2>Welcome to Khronos!</h2>
                <Link to="/gnss">Navigation</Link>
                <Link to="/tuner">Tuner</Link>
                <Link to="/clock">Time & Date</Link>
                <Link to="/settings">Settings</Link>
            </div>
        )
    }

}

export default Dashboard
