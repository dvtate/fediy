

/////////////////////////////////////////
// Light/dark mode themes
/////////////////////////////////////////

// Dark/light mode switch
const themeToggle = document.getElementById('theme-toggle');

// Change the theme used across the site
function setTheme(t/*: 'dark' | 'light'*/) {
    // Set to 'dark'
    if (t === 'dark') {
        localStorage.setItem('theme', t);
        document.getElementsByTagName('html')[0].setAttribute('data-theme', t);
        themeToggle.classList.remove('fa-moon');
        themeToggle.classList.add('fa-sun');
        return;
    }

    // Set to 'light'
    localStorage.setItem('theme', t);
    document.getElementsByTagName('html')[0].setAttribute('data-theme', t);
    themeToggle.classList.remove('fa-sun');
    themeToggle.classList.add('fa-moon');
}

// Toggle theme on click
if (themeToggle)
    themeToggle.onclick = () =>
        setTheme(themeToggle.classList.contains('fa-sun') ? 'light' : 'dark');

// If they have a preferred theme already
const themeSelection = localStorage.getItem('theme');// as 'dark' | 'light' | null;
if (themeSelection)
    setTheme(themeSelection);