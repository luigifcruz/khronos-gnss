import React, { Component } from 'react'
import '../styles/App.scss'

class App extends Component {

    render() {
        return (
            <div>
                {this.props.children}
            </div>
        )
    }

}

export default App
