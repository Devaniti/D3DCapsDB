"use strict";

const fs = require('fs')
const express = require('express')
const sqlite3 = require('sqlite3').verbose();

const databasePath = 'Database/main.db'
const databaseSchemaPath = 'DBSchema.sql'
const apiPort = 50853
const propertyWhitelist = require("./property_whitelist.json");

function isObjectAllowedInDB(inObj)
{
    for (const property in inObj) {
        if (!propertyWhitelist.includes(property))
        {
            console.log(`Object disallowed for property ${property}`)
            return false
        }

        var propertyValue = inObj[property];
        if (typeof(propertyValue) != 'number' && typeof(propertyValue) != 'string' && propertyValue != null)
        {
            console.log(`Object disallowed for property ${property} of type ${typeof(propertyValue)}`)
            return false
        }
    }
    return true;
}

function convertToSqlQuery(inObj)
{
    return {
        columnsString: "(" + Object.keys(inObj).join(",") + ")",
        valuesString: "(" + Object.keys(inObj).map(e=>"?").join(",") + ")",
        valuesArray: Object.keys(inObj).map(e=>inObj[e])
    }
}

console.log('Initializing')

const api = express()
let db = new sqlite3.Database(databasePath)
console.log(`Opened database ${databasePath}`)

// Initialize DB if not already initialized
db.run(fs.readFileSync(databaseSchemaPath).toString())
console.log(`Executed sql file ${databaseSchemaPath}`)

api.use(express.json());
api.use(express.urlencoded({ extended: true }));

api.get('/', (req, res) => {
    res.send('Server is up')
})

api.get('/get_submission', async (req, res) => {
    if (!req.query.ID)
    {
        res.status(400)
        res.send('ID is missing')
        return
    }

    db.all("SELECT * FROM Submissions WHERE ID = ?", [req.query.ID], (err, rows) => {
        res.send(JSON.stringify(rows))
    });
})

api.get('/get_all_submissions', async (req, res) => {
    db.all("SELECT * FROM Submissions", (err, rows) => {
        res.send(JSON.stringify(rows))
    });
})

api.post('/post_submission', async (req, res) => {
    const newSubmission = req.body
    if (!isObjectAllowedInDB(req.body))
    {
        res.status(400)
        res.send('Bad submission format')
        return
    }
    const sqlQueryParts = convertToSqlQuery(newSubmission)
    const sqlQuery = "INSERT INTO Submissions " + sqlQueryParts.columnsString + " Values " + sqlQueryParts.valuesString + ";"
    db.run(sqlQuery, sqlQueryParts.valuesArray, function(err){
        if (err)
        {
            res.status(500)
            res.send('DB Error')
        }
        console.dir(`Inserted submission ID ${this.lastID}`)
        res.send("" + this.lastID)
    })
})

api.listen(apiPort, () => {
    console.log(`Started server on port ${apiPort}`)
})
