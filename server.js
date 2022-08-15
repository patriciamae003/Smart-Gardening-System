const express = require('express');
const app = express();
const path = require("path");

app.set('PORT', process.env.PORT || 3001);

app.use(express.static(path.join(__dirname, "frontend", "build")));

app.get("*", (req, res) => {
     res.sendFile(path.join(__dirname, "frontend", "build", "index.html"));
});


app.listen(app.get('PORT'), () =>
    console.log(`Server running on port ${app.get('PORT')}`),
);
