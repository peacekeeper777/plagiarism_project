const express = require('express');
const multer = require('multer');
const { exec } = require('child_process');
const path = require('path');

const app = express();

const upload = multer({ dest: 'uploads/' });

app.use(express.static(path.join(__dirname, '../frontend')));

app.post('/compare', upload.fields([{ name: 'file1' }, { name: 'file2' }]), (req, res) => {
    const file1 = req.files['file1'][0].path;
    const file2 = req.files['file2'][0].path;
    const exePath = path.join(__dirname, '../cpp_engine/a.exe');
    const command = `"${exePath}" ${file1} ${file2}`;

    exec(command, (err, stdout, stderr) => {
        if (err) {
            console.error(err);
            return res.json(JSON.parse(stdout));
        }
        res.send(stdout);
    });
});

app.listen(3000, () => {
    console.log("Server running at http://localhost:3000");
});