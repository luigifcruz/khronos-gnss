import HeatmapLayer from "react-google-maps/lib/components/visualization/HeatmapLayer"
import { TimeSeries, Index, TimeEvent, TimeRange } from "pondjs"
import { CopyToClipboard } from 'react-copy-to-clipboard'
import { compose, withProps } from 'recompose'
import { timeFormat } from 'd3-time-format'
import React, { Component } from 'react'
import * as rxa from '../redux/actions'
import { connect } from 'react-redux'
import '../styles/Gnss.scss'
import Ring from "ringjs"

import {
    LineChart,
    Baseline,
    ChartContainer,
    ChartRow,
    EventChart,
    YAxis,
    Charts,
    styler
} from "react-timeseries-charts";

import {
  withScriptjs,
  withGoogleMap,
  GoogleMap,
  Marker
} from "react-google-maps";

const MyMapComponent = compose(
  withProps({
    googleMapURL: "https://maps.googleapis.com/maps/api/js?key=AIzaSyA8-WppouVH_aiun-Poq89TAxhRhIj1y4Y&libraries=visualization",
    loadingElement: <div style={{ height: `100%` }} />,
    containerElement: <div style={{ height: `380px` }} />,
    mapElement: <div style={{ height: `100%` }} />,
  }),
  withScriptjs,
  withGoogleMap
)((props) =>
    <GoogleMap
        defaultZoom={16} 
        options={{ scrollwheel: true }} 
        defaultCenter={{ lat: props.lat, lng: props.lng }} >
        <Marker position={{ lat: props.lat, lng: props.lng }} />
        <HeatmapLayer data={props.loc.toArray()}/>
    </GoogleMap>
)

const style = styler([
    { key: "altitude", color: "#297373", width: 2 },
    { key: "speed", color: "#F24333", width: 2 }
]);

class Gnss extends Component {

    constructor(props) {
        super(props);
        this.state = {
            plot_data: new Ring(2000),
            location: new Ring(100)
        };
    }

    digestData() {
        let { state } = this.props;
        const { plot_data } = this.state;

        plot_data.push(new TimeEvent(new Date(), {
            altitude: state.altitude,
            speed: state.ground_speed
        }));

        this.setState({ plot_data });

        const { location } = this.state;
        const htmapPoint = {
            location: new google.maps.LatLng(
                state.latitude,
                state.longitude
            ), 
            weight: 1
        };
        location.push(htmapPoint);
        this.setState({ location });
    }

    componentDidMount() {
        this.timer = setInterval(() => { this.digestData() }, 1000)
    }

    componentWillUnmount() {
        clearInterval(this.timer);
    }

    render() {
        const plot_data = new TimeSeries({
            name: "plot_data",
            columns: ["time", "data"],
            events: this.state.plot_data.toArray()
        });

        let { gnss_fix_type, gnss_fix_quality } = this.props.state;

        let gnssFixType, gnssFixTypeColor;
        if (gnss_fix_type === 1 || gnss_fix_type === 0) {
            gnssFixType = "NO FIX"
            gnssFixTypeColor = "#E54B4B"
        } else if (gnss_fix_type === 2) {
            gnssFixType = "2D FIX"
            gnssFixTypeColor = "#FDE74C"
        } else if (gnss_fix_type === 3) {
            gnssFixType = "3D FIX"
            gnssFixTypeColor = "#4EA699"
        }

        let gnssFixQuality, gnssFixQualityColor;
        if (gnss_fix_quality === 0) {
            gnssFixQuality = "NO FIX"
            gnssFixQualityColor = "#E54B4B"
        } else if (gnss_fix_quality === 1) {
            gnssFixQuality = "GNSS"
            gnssFixQualityColor = "#6A7FDB"
        } else if (gnss_fix_quality === 2) {
            gnssFixQuality = "DGNSS"
            gnssFixQualityColor = "#4EA699"
        }

        return (
            <div className="Gnss">
                <h2>GNSS Dashboard</h2>
                <div className="Map">
                    <MyMapComponent
                        lat={this.props.state.latitude}
                        lng={this.props.state.longitude}
                        loc={this.state.location} />
                </div>
                <div className="Variables">
                    <div>
                        <div className="FloatNarrow NoHover">
                            <p style={{ color: gnssFixTypeColor }}>{gnssFixType}</p>
                            <h5>FIX TYPE</h5>
                        </div>
                        <div className="FloatTiny NoHover">
                            <p>{this.props.state.sat_count_glonass}</p>
                            <h5>GLONASS SATS</h5>
                        </div>
                        <CopyToClipboard text={this.props.state.altitude}>
                            <div className="FloatNarrow">
                                <p>{this.props.state.altitude.toFixed(2)}</p>
                                <h5>ALTITUDE (m)</h5>
                            </div>
                        </CopyToClipboard>
                        <CopyToClipboard text={this.props.state.latitude}>
                            <div className="FloatWide">
                                <p>{this.props.state.latitude.toFixed(6)}</p>
                                <h5>LATITUDE</h5>
                            </div>
                        </CopyToClipboard>
                    </div>
                    <div>
                        <div className="FloatNarrow NoHover">
                            <p style={{ color: gnssFixQualityColor }}>{gnssFixQuality}</p>
                            <h5>FIX QUALITY</h5>
                        </div>
                        <div className="FloatTiny NoHover">
                            <p>{this.props.state.sat_count_gps}</p>
                            <h5>GPS SATS</h5>
                        </div>
                        <CopyToClipboard text={this.props.state.ground_speed}>
                            <div className="FloatNarrow">
                                <p>{this.props.state.ground_speed.toFixed(2)}</p>
                                <h5>SPEED (km/h)</h5>
                            </div>
                        </CopyToClipboard>
                        <CopyToClipboard text={this.props.state.longitude}>
                            <div className="FloatWide">
                                <p>{this.props.state.longitude.toFixed(6)}</p>
                                <h5>LONGITUDE</h5>
                            </div>
                        </CopyToClipboard>
                    </div>
                </div>
                <div className="Chart">
                    <ChartContainer 
                        format="relative"
                        format={timeFormat("%H:%M")}
                        timeRange={plot_data.timerange() || new TimeRange(0, 0)}>
                        <ChartRow height="175">
                            <YAxis
                                id="y"
                                min={plot_data.min("altitude")}
                                max={plot_data.max("altitude")}
                                label="Altitude (m)"
                                format=".1f"
                                type="linear"/>
                            <Charts>
                                <LineChart axis="y" style={style} columns={["altitude"]} series={plot_data} interpolation="curveBasis" />
                                <Baseline axis="y" value={plot_data.avg("altitude")} label="Avg"/>
                            </Charts>
                        </ChartRow>
                        <ChartRow height="175">
                            <YAxis
                                id="y"
                                min={plot_data.min("speed")}
                                max={plot_data.max("speed")}
                                label="Ground Speed (kph)"
                                format=".1f"
                                type="linear"/>
                            <Charts>
                                <LineChart axis="y" style={style} columns={["speed"]} series={plot_data} interpolation="curveBasis" />
                            </Charts>
                        </ChartRow>
                    </ChartContainer>
                </div>
            </div>
        )
    }

}

Gnss.propTypes = rxa.props
export default connect(rxa.mapStateToProps)(Gnss)  
