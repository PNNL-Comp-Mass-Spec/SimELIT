
// import "./css/layout.css";

import Container from '@material-ui/core/Container';

import React from "react";
import AppBar from '@material-ui/core/AppBar';
import Button from '@material-ui/core/Button';
import Tab from "@material-ui/core/Tab";
import TabContext from '@material-ui/lab/TabContext';
import TabList from '@material-ui/lab/TabList';
import TabPanel from '@material-ui/lab/TabPanel';
import Select from '@material-ui/core/Select';
import MenuItem from '@material-ui/core/MenuItem';
import FormHelperText from '@material-ui/core/FormHelperText';
import FormControl from '@material-ui/core/FormControl';
import InputLabel from '@material-ui/core/InputLabel';
import Typography from "@material-ui/core/Typography";
import Grid from '@material-ui/core/Grid';

import { theme, buttonTheme } from "./theme.js";
import { MuiThemeProvider } from "@material-ui/core/styles";

import Simulation from "./views/Simulation.js";
import FileTreeView from "./views/FileTreeView.js";
import LogTerminal from "./views/LogTerminal.js";
import sendCommand from "./send.js";

import io from 'socket.io-client'

const namespace = '/openfoam';
const socket = io('http://'+document.domain+':5000'+namespace);


class Layout extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      logs : [],
      logStrings: "",
      value: "1",
      case: "",
      solver: "",
      solver_exe: "",
      cases: {},
      solvers: {}
    };
  }

  componentDidMount() {
    socket.on('connect', () => {
      console.log("CONNECTED!");
      setTimeout(
        () => { socket.emit('join_room'); },
        1000
      )
    });

    socket.on('ack', (data) => {
      console.log(data);
    });

    socket.on('status', (data) => {
      const logStrings2 = this.state.logStrings + data.msg;
      const logs2 = this.state.logs.concat(data.msg.split("\n").map(line => [data.source, line]));
      logs2.key = 'key_' + (this.state.logs.length + 1)
      console.log(data);
      this.setState({
        logs: logs2,
        logStrings: logStrings2
      })
    });

    fetch("/datafolders", {
      method: "POST",
    })
    .then(resp => {
      return resp.json()
    }) 
    .then(data => {
      this.setState({
        cases : data.cases,
        solvers : data.solvers
      });
      console.log(this.state)
    })
    .catch((error) => {
      console.log(error, "catch the hoop")
    });
  }

  componentWillUnmount() {
    socket.close();
  }

  handleChange = (event, newValue) => {
    this.setState({ value: newValue });
  };

  onCaseChange = (event, k) => {
    console.log(event.target.value);
    this.setState({ case: event.target.value });
  };

  onSolverChange = (event, k) => {
    console.log(event.target.value);
    this.setState({ solver: event.target.value });
    this.setState({ solver_exe: this.state.solvers[event.target.value] });
  };

  onClickWMake = (event, k) => {
    // sendCommand("wclean /usr/src/temp/" + this.state.solver + " && wmake /usr/src/temp/" + this.state.solver)
    sendCommand("wclean all /usr/src/data/" + this.state.solver + " && wmake all /usr/src/data/" + this.state.solver)
  };

  render() {
    console.log(this.state);
    return (
      <Container maxWidth="lg">
        <TabContext value={this.state.value}>
          <MuiThemeProvider theme={theme}>
          <AppBar position="static">
            <TabList onChange={this.handleChange} aria-label="Simple UI for openfoam-gui">
              <Tab label="OpenFOAM GUI" value="1" />
              <Tab label="Edit Cases" value="2" />
              <Tab label="Simulation" value="3" />
            </TabList>
          </AppBar>
          </MuiThemeProvider>
          <TabPanel value="1">
            <Typography variant="h4">OpenFOAM-GUI</Typography>
            <Grid container spacing={2}>
            <Grid item>
            <FormControl className="select-solver">
              <InputLabel id="solver">Solver</InputLabel>
              <Select
                labelId="solver"
                id="select-solver"
                value={this.state.solver}
                onChange={this.onSolverChange}
              >
                {Object.keys(this.state.solvers).map(log => <MenuItem key={log} value={log}>{log}</MenuItem>)}
              </Select>
              <FormHelperText>Select your solver.</FormHelperText>
            </FormControl>
            </Grid>
            <Grid item>
            <FormControl className="select-case">
              <InputLabel id="case">Case</InputLabel>
              <Select
                labelId="case"
                id="select-case"
                value={this.state.case}
                onChange={this.onCaseChange}
              >
                {Object.keys(this.state.cases).map(log => <MenuItem key={log} value={log}>{log}</MenuItem>)}
              </Select>
              <FormHelperText>Select your case.</FormHelperText>
            </FormControl>
            </Grid>
            <Grid item>
            <MuiThemeProvider theme={buttonTheme}>
              <Button color="primary" onClick={this.onClickWMake}>WMake</Button>
            </MuiThemeProvider>
            </Grid>
            </Grid>
          </TabPanel>
          <TabPanel value="2"><FileTreeView solver={this.state.solver} case={this.state.case}/></TabPanel>
          <TabPanel value="3"><Simulation solver={this.state.solver_exe}  case={this.state.case}/></TabPanel>
        </TabContext>
        <LogTerminal logs={this.state.logs}/>
      </Container>
    );
  }
}

export default Layout;
