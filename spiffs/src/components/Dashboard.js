import React, { Component } from 'react'
import { Link } from 'react-router-dom'
import '../styles/Dashboard.scss'

class Dashboard extends Component {

    render() {
        return (
            <div className="Dashboard">
                <p>Welcome to Khronos!</p>
                <Link to="/gnss">Navigation</Link>
                <Link to="/tuner">Tuner</Link>
                <Link to="/clock">Time & Date</Link>
                <Link to="/settings">Settings</Link>
            </div>
        )
    }

}

export default Dashboard
