import React from 'react';
// import ReactDOM from 'react-dom';
// import {includes} from 'lodash';

// import { NavLink } from "react-router-dom";

// import {Treebeard, decorators} from 'react-treebeard';

import data from '../data';

import { theme, buttonTheme } from "../theme.js";
import { MuiThemeProvider } from "@material-ui/core/styles";


import TreeView from '@material-ui/lab/TreeView';
import ExpandMoreIcon from '@material-ui/icons/ExpandMore';
import ChevronRightIcon from '@material-ui/icons/ChevronRight';
import TreeItem from '@material-ui/lab/TreeItem';
import Paper from '@material-ui/core/Paper';
import Grid from '@material-ui/core/Grid';
import sendCommand from "../send.js";
import Button from '@material-ui/core/Button';

class FileTreeView extends React.Component {
    constructor(props) {
        super(props);
        
        data.name = props.case
        this.state = {
            tree: data,
            file: {},
            content: ""
        };
    }

    componentDidMount() {
        const requestOptions = {
            method: 'POST',
            headers: {
              'Content-Type': 'application/json'
            },
            body: JSON.stringify({ case: this.props.case })
        };

        fetch("/files", requestOptions)
        .then(res => res.json())
        .then((result) => {
            // console.log(result)
            const ft = this.state.tree
            ft.children = result
            this.setState({ tree: ft })
        });

        this.setState({ content: "" })
    }

    onNodeSelect(event, value) {
        if (value===1) return;
        var split = value.split("/");
        if (split.length < 2) return;
        
        const body = {
            case: this.props.case,
            folder: split[0],
            file: split[1],
        };

        var file = {
            parent: split[0],
            name: split[1]
        };

        fetch("/file", {
            method: "POST",
            headers: {
              'Content-Type': 'application/json'
            },
            body: JSON.stringify(body),
        })
        .then(res => res.json())
        .then(res => JSON.stringify(res))
        .then((result) => {
            const filedata = JSON.parse(result).data;
            this.setState({ file: file });
            this.setState({ content: filedata });
        });
        this.setState({ content: "Loading ..." });
    }

    save = () => {
        const body = {
            case: this.props.case,
            folder: this.state.file.parent,
            file: this.state.file.name,
            data: this.state.content,
        };
        console.log(body)

        fetch("/save", {
          method: "POST",
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify(body),
        })
        .then(resp => {
          return resp.json()
        }) 
        .catch((error) => {
          console.log(error, "catch the hoop")
        })
      }

    setFields = () => {
        sendCommand("setFields -case /usr/src/temp/" + this.props.case)
    }

    renderTree = (nodes) => (
        <TreeItem key={nodes.id+""} nodeId={nodes.id+""} label={nodes.name}>
            {Array.isArray(nodes.children) ? nodes.children.map((node) => this.renderTree(node)) : null}
        </TreeItem>
    );

    render() {
        const {tree, content} = this.state;
        return (
        <>
        <MuiThemeProvider theme={theme}>
            <Grid container spacing={2}>
                <Grid item xs={6} sm={3}>
                    <Paper>
                    <TreeView
                        defaultCollapseIcon={<ExpandMoreIcon />}
                        defaultExpanded={['root']}
                        defaultExpandIcon={<ChevronRightIcon />}
                        onNodeSelect={this.onNodeSelect.bind(this)}
                    >
                        {this.renderTree(tree)}
                    </TreeView>
                    </Paper>
                </Grid>
                <Grid item xs={6} sm={9}>
                    <Grid container direction='column'>
                        <Grid item>
                        <textarea cols='100' rows="45" type="text"
                            spellCheck="false" value={content}
                            onChange={e => this.setState({ content: e.target.value })}
                        />
                        </Grid>
                        <Grid item>
                        <MuiThemeProvider theme={buttonTheme}>
                            <Button color="primary" onClick={this.save}>Save</Button>
                            <Button color="primary" onClick={this.setFields}>Set Fields</Button>
                        </MuiThemeProvider>
                        </Grid>
                  </Grid>
                </Grid>
              </Grid>
        </MuiThemeProvider>
        </>
            
        );
    }
}

export default FileTreeView;