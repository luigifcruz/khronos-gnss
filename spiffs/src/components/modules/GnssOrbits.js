import React, { Component } from 'react'
import * as d3 from "d3";

import '../../styles/Orbits.scss'

Number.prototype.pad = function(size) {
    var s = String(this);
    while (s.length < (size || 2)) {s = "0" + s;}
    return s;
}

class Orbits extends Component {

    constructor(props) {
        super(props);
        this.state = {
            gnss_position: [[0,0,"--",0,"--","--"]]
        }
    }

    componentWillReceiveProps(newProps) {
        if (newProps.state.gnss_sat_info != this.props.state.gnss_sat_info) {
            let gnss_position = [];

            newProps.state.gnss_sat_info.gps.forEach((sat) => {
                gnss_position.push([sat.a, sat.e, `G${sat.n.pad()}`, sat.s, "GPS", "#009900"]);
            });
            newProps.state.gnss_sat_info.glonass.forEach((sat) => {
                gnss_position.push([sat.a, sat.e, `G${sat.n.pad()}`, sat.s, "GLONASS", "#F21B3F"]);
            });
            newProps.state.gnss_sat_info.galileo.forEach((sat) => {
                gnss_position.push([sat.a, sat.e, `G${sat.n.pad()}`, sat.s, "Galileo", "#4392F1"]);
            });

            this.setState({ gnss_position });
            this.draw();
        }
    }

    componentDidMount() {
        this.sats = d3.select(this.node).append("svg");

        var width = this.node.clientWidth;
        var height = this.node.clientHeight;

        this.projection = d3.geoProjection(flippedStereographic)
            .scale(width * 0.40)
            .clipAngle(130)
            .rotate([0, -90])
            .translate([width / 2 + 0.5, height / 2 + 0.5])
            .precision(1);

        let projection = this.projection
        let sats = this.sats

        var path = d3.geoPath(projection);

        sats.append("path")
            .datum(d3.geoCircle().center([0, 90]).radius(90))
            .attr("stroke-width", 1.5)
            .attr("d", path);

        sats.append("path")
            .datum(d3.geoGraticule())
            .attr("stroke-width", 0.15)
            .attr("d", path);

        sats.append("g")
            .selectAll("line")
            .data(d3.range(360))
            .enter().append("line")
            .each(function(d) {
                var p0 = projection([d, 0]),
                    p1 = projection([d, d % 10 ? -1 : -2]);

                d3.select(this)
                    .attr("x1", p0[0])
                    .attr("y1", p0[1])
                    .attr("x2", p1[0])
                    .attr("y2", p1[1]);
            });

        sats.append("g")
            .attr("fill", "black")
            .attr("stroke", "none")
            .selectAll("text")
            .data(d3.range(0, 360, 10))
            .enter().append("text")
            .each(function(d) {
                var p = projection([d, -4]);
                d3.select(this).attr("x", p[0]).attr("y", p[1]);
            })
            .attr("dy", "0.35em")
            .text(function(d) { return d === 0 ? "N" : d === 90 ? "E" : d === 180 ? "S" : d === 270 ? "W" : d + "°"; })
            .data(d3.range(0, 360, 90), function(d) { return d; })
            .attr("font-weight", "bold")
            .attr("font-size", 14);

        sats.append("g")
            .attr("fill", "#A3ACA9")
            .attr("stroke", "none")
            .selectAll("text")
            .data(d3.range(10, 91, 10))
            .enter().append("text")
            .each(function(d) {
                var p = projection([0, d]);
                d3.select(this).attr("x", p[0]).attr("y", p[1]);
            })
            .attr("dy", "-0.4em")
            .text(function(d) { return d + "°"; });
    }

    draw() {
        let projection = this.projection
        let sats = this.sats

        sats.select('g.satellites').remove();
        
        let points = sats
            .insert("g")
            .attr("class", "satellites")
            .selectAll('g.satellite')
            .data(this.state.gnss_position)
            .enter()
            .append('g')
            .attr("transform", function(d) {
                var p = projection(d);
                return 'translate(' + p[0] + ', ' + p[1] + ')';
            });
            
        points
            .attr('class', 'satellite')
            .append("circle")
            .attr("stroke", function(d) {
                return d[3] > 0 ? "transparent" : d[5];
            })
            .attr("r", function(d) {
                return d[3] > 0 ? d[3]*0.5 : 3;
            })
            .attr("fill", function(d) {
                return d[3] > 0 ? d[5] : "transparent";
            });

        points
            .attr("r", 50)
            .append("text")
            .attr("class", "sats-label")
            .attr('dy', function(d) {
                return d[3] > 0 ? `${10+(d[3]/2)}px` : "1.3em";
            })
            .attr('dx', function(d) {
                return d[3] > 0 ? `${3+(d[3]/2)}px` : "0.7em";
            })
            .text(function(d){return d[2]})

    }

    render() {
        return (
            <div className="Orbits">
                <div className="Drawing" ref={node => this.node = node}></div>
                <div className="Variables">
                    <table>
                        <tr>
                            <th>PRN</th>
                            <th>GNSS</th>
                            <th>Elevation</th> 
                            <th>Azimuth</th>
                            <th>SNR</th>
                        </tr>
                        {this.state.gnss_position.map(function(sat, i){
                            return (
                                <tr>
                                    <td>{sat[2]}</td>
                                    <td>{sat[4]}</td>
                                    <td>{sat[1].pad() + "°"}</td> 
                                    <td>{sat[0].pad(3) + "°"}</td>
                                    <td>{sat[3].pad()}</td>
                                </tr>
                            )
                        })}
                    </table>
                </div>
            </div>
        )
    }

}

function flippedStereographic(x, y)  {
    var cx = Math.cos(x), cy = Math.cos(y), k = 1 / (1 + cx * cy);
    return [k * cy * Math.sin(x), -k * Math.sin(y)];
}

export default Orbits
