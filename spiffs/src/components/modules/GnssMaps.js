import HeatmapLayer from "react-google-maps/lib/components/visualization/HeatmapLayer"
import { compose, withProps } from 'recompose'
import React, { Component } from 'react'

import {
  withScriptjs,
  withGoogleMap,
  GoogleMap,
  Marker
} from "react-google-maps";

const googleMapURL = "https://maps.googleapis.com/maps/api/js?key=AIzaSyA8-WppouVH_aiun-Poq89TAxhRhIj1y4Y&libraries=visualization"

const MapComponent = compose(
    withProps({
        googleMapURL,
        loadingElement: <div style={{ height: '100%' }} />,
        containerElement: <div style={{ height: '100%' }} />,
        mapElement: <div style={{ height: '100%' }} />,
    }),
    withScriptjs,
    withGoogleMap
)((props) =>
    <GoogleMap
        defaultZoom={16} 
        options={{ scrollwheel: true }} 
        center={{ lat: props.lat, lng: props.lng }} >
        <Marker position={{ lat: props.lat, lng: props.lng }} />
        <HeatmapLayer data={props.mapdata.toArray()}/>
    </GoogleMap>
)

class Maps extends Component {

    render() {
        return (
            <div className="Map">
                <MapComponent
                    lat={this.props.state.latitude}
                    lng={this.props.state.longitude}
                    mapdata={this.props.mapdata} />
            </div>
        )
    }

}

export default Maps
