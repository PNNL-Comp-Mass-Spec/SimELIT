import { red } from '@material-ui/core/colors';
import { createMuiTheme } from '@material-ui/core/styles';

// A custom theme for this app
const theme = createMuiTheme({
    palette: {
        primary: {
            main: '#1976d2',
        },
        secondary: {
            main: '#fff',
        },
        error: {
            main: red.A400,
        },
        background: {
            default: '#fff',
        },
    }

});

const buttonTheme = createMuiTheme({
    typography: {
        button: {
            background: 'linear-gradient(45deg, #FE6B8B 30%, #FF8E53 90%)',
            textTransform: "none",
            boxShadow: '0 3px 5px 2px rgba(255, 105, 135, .3)',
            border: 0,
            borderRadius: 3,
            padding: '0 30px',
            'margin-right': '1px',
            'margin-left': '2px',
        }
    },
    palette: {
        primary: {
            main: '#fff',
        },
      }
});
// const buttonTheme = createMuiTheme({
//     palette: {
//         primary: {
//             main: '#1976d2',
//         },
//         secondary: {
//             main: '#fff',
//         },
//         error: {
//             main: red.A400,
//         },
//         background: {
//             default: '#000',
//         },
//         text: {
//             primary: "#fff",
//         },
//         action: {
//             active: "rgba(0, 0, 0, 0.7)",
//             hover: "rgba(0, 0, 0, 0.4)",
//             hoverOpacity: 0.4,
//         }

//     },
//     typography: {
//         button: {
//             background: "#1976d2",
//             textTransform: "none"
//         }
//     }
// });

export { theme, buttonTheme };

