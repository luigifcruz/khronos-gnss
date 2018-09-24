import React, { Component } from 'react'
import { TimeSeries, Index, TimeEvent, TimeRange } from "pondjs"
import { csvFormat } from 'd3-dsv'

import {
    LineChart,
    Baseline,
    ChartContainer,
    ChartRow,
    EventChart,
    YAxis,
    Charts,
    styler,
    Resizable,
    EventMarker
} from "react-timeseries-charts";

const styleNormal = styler([
    { key: "altitude", color: "#C6C6C6", width: 1 },
    { key: "ground_speed", color: "#C6C6C6", width: 1 }
]);

const styleInterp = styler([
    { key: "altitude", color: "#297373", width: 2 },
    { key: "ground_speed", color: "#F24333", width: 2 }
]);

const baselineStyle = { 
    label: { fill: "#ACA9A9", fontWeight: 700, fontSize: 14 }, 
    line: { stroke: "#ACA9A9", strokeWidth: 2 }
}

class Chart extends Component {

    constructor(props) {
        super(props);
        this.state = {
            trackerAltValue: null,
            trackerSpdValue: null,
            trackerEvent: null,
            pausedData: null,
            baselineMax: false,
            baselineAvg: false,
            baselineMin: false
        };
    }

    handleTrackerChanged(t) {
        if (t) {
            const trackerEvent = this.plot_data.atTime(t);
            if (trackerEvent == undefined) { return; }
            const trackerAltValue = `${trackerEvent.get("altitude").toFixed(2)} m`;
            const trackerSpdValue = `${trackerEvent.get("ground_speed").toFixed(2)} km/h`;
            this.setState({ trackerAltValue, trackerSpdValue, trackerEvent });
        } else {
            this.setState({ trackerAltValue: null, trackerSpdValue: null, trackerEvent: null });
        }
    }

    handlePauseEvent() {
        this.setState({
            pausedData: (this.state.pausedData) ? null : this.props.history.toArray()
        });
    }

    handleAvg() {
        this.setState({ baselineAvg: !this.state.baselineAvg });
    }

    handleMax() {
        this.setState({ baselineMax: !this.state.baselineMax });
    }
    
    handleMin() {
        this.setState({ baselineMin: !this.state.baselineMin });
    }

    handleExport() {
        let data = this.plot_data.toJSON().points;
        data.unshift(this.plot_data.toJSON().columns);
        let fn = `khronos_export_${data[2][0]}_${data[data.length-1][0]}.csv`
        let f = new File([csvFormat(data)], fn, {type: "text/csv"});
        let a = window.document.createElement('a');
        a.href = window.URL.createObjectURL(f);
        a.download = fn;
        document.body.appendChild(a);
        a.click();
        document.body.removeChild(a);
    }

    render() {
        let { pausedData, baselineMax, baselineMin, baselineAvg } = this.state;

        this.plot_data = new TimeSeries({
            name: "plot_data",
            columns: ["time", "data"],
            events: (pausedData) ? pausedData : this.props.history.toArray()
        });

        const markerData = [
            { label: "Altitude", value: this.state.trackerAltValue },
            { label: "Speed", value: this.state.trackerSpdValue }
        ]

        const avg = {
            alt: this.plot_data.avg("altitude") || 0,
            spd: this.plot_data.avg("ground_speed") || 0
        }

        const max = {
            alt: this.plot_data.max("altitude") || 0,
            spd: this.plot_data.max("ground_speed") || 0
        }

        const min = {
            alt: this.plot_data.min("altitude") || 0,
            spd: this.plot_data.min("ground_speed") || 0
        }

        return (
            <div className="Chart">
                <Resizable>
                    <ChartContainer 
                        format="relative"
                        format="%H:%M"
                        onTrackerChanged={this.handleTrackerChanged.bind(this)}
                        timeRange={this.plot_data.timerange() || new TimeRange(0, 0)}>
                        <ChartRow height="175">
                            <YAxis
                                id="y"
                                min={this.plot_data.min("altitude")}
                                max={this.plot_data.max("altitude")}
                                label="Altitude (m)"
                                format=".1f"
                                type="linear"/>
                            <Charts>
                                <LineChart axis="y" style={styleNormal} columns={["altitude"]} series={this.plot_data} />
                                <LineChart axis="y" style={styleInterp} columns={["altitude"]} series={this.plot_data} interpolation="curveBasis" />
                                <Baseline visible={baselineAvg} style={baselineStyle} axis="y" value={avg.alt} label={`Avg (${avg.alt.toFixed(2)} m)`}/>
                                <Baseline visible={baselineMax} style={baselineStyle} axis="y" value={max.alt} label={`Max (${max.alt.toFixed(2)} m)`}/>
                                <Baseline visible={baselineMin} style={baselineStyle} axis="y" value={min.alt} label={`Min (${min.alt.toFixed(2)} m)`}/>
                                <EventMarker
                                    type="flag"
                                    axis="y"
                                    column="altitude"
                                    event={this.state.trackerEvent}
                                    info={markerData}
                                    infoTimeFormat="%H:%M:%S"
                                    infoWidth={120}
                                    infoHeight={40}
                                    markerRadius={2}
                                />
                            </Charts>
                        </ChartRow>
                        <ChartRow height="175">
                            <YAxis
                                id="y"
                                min={this.plot_data.min("ground_speed")}
                                max={this.plot_data.max("ground_speed")}
                                label="Ground Speed (km/h)"
                                format=".1f"
                                type="linear"/>
                            <Charts>
                                <LineChart axis="y" style={styleNormal} columns={["ground_speed"]} series={this.plot_data} />
                                <LineChart axis="y" style={styleInterp} columns={["ground_speed"]} series={this.plot_data} interpolation="curveBasis" />
                                <Baseline visible={baselineAvg} style={baselineStyle} axis="y" value={avg.spd} label={`Avg (${avg.spd.toFixed(2)} km/h)`}/>
                                <Baseline visible={baselineMax} style={baselineStyle} axis="y" value={max.spd} label={`Max (${max.spd.toFixed(2)} km/h)`}/>
                                <Baseline visible={baselineMin} style={baselineStyle} axis="y" value={min.spd} label={`Min (${min.spd.toFixed(2)} km/h)`}/>
                            </Charts>
                        </ChartRow>
                    </ChartContainer>
                </Resizable>
                <center>
                    <button 
                        className={(pausedData) ? "Active" : null} 
                        onClick={this.handlePauseEvent.bind(this)}>
                        {(pausedData) ? "RESUME" : "PAUSE"}
                    </button>
                    <button 
                        className={(baselineAvg) ? "Small Active" : "Small"} 
                        onClick={this.handleAvg.bind(this)}>
                        AVG
                    </button>
                    <button 
                        className={(baselineMax) ? "Small Active" : "Small"} 
                        onClick={this.handleMax.bind(this)}>
                        MAX
                    </button>
                    <button 
                        className={(baselineMin) ? "Small Active" : "Small"} 
                        onClick={this.handleMin.bind(this)}>
                        MIN
                    </button>
                </center>
            </div>
        )
    }

}

export default Chart
