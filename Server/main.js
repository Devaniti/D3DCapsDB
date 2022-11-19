"use strict";

const fs = require('fs')
const https = require('https')
const http = require('http')
const express = require('express')
const cors = require('cors');
const Database = require('better-sqlite3');

const databasePath = 'Database/main.db'
const databaseSchemaPath = 'DBSchema.sql'
const apiPort = 50853
const requiredProperites = require("./StructureDesc/required_properites.json")
const submitUniqueProperites = require("./StructureDesc/submit_unique_properties.json")
const allProperties = require("./StructureDesc/all_properites.json")

function isObjectAllowedInDB(inObj)
{
    return requiredProperites.every(p => inObj[p] != null)
}

function convertToSqlInsertQuery(tableName, propertyArray)
{
    let columnsString = "(" + propertyArray.map(e=>`"${e}"`).join(",") + ")"
    let valuesString = "(" + propertyArray.map(e=>"?").join(",") + ")"
    let sqlQuery = `INSERT INTO ${tableName} ${columnsString} Values ${valuesString};`
    return sqlQuery
}

function convertToSqlSelectIDQuery(tableName, propertyArray, suffix)
{
    let filterString = propertyArray.map(e => `"${e}" = ?`).join(" AND ")
    let sqlQuery = `Select ID from ${tableName} WHERE ${filterString} ${suffix};`
    return sqlQuery
}

function convertObjectToArrayOfValues(propertyList, inObj)
{
    return propertyList.map(e=>inObj[e])
}

console.log('Initializing')

const api = express()
let db = new Database(databasePath)
console.log(`Opened database ${databasePath}`)

// Initialize DB if not already initialized
db.exec(fs.readFileSync(databaseSchemaPath).toString())
console.log(`Executed sql file ${databaseSchemaPath}`)

api.use(express.json());
api.use(express.urlencoded({ extended: true }));
api.use(cors());

api.get('/', (req, res) => {
    res.send('Server is up')
})

const getSubmissionStatement = db.prepare("SELECT * FROM Submissions WHERE ID = ?");
api.get('/get_submission', (req, res) => {
    if (!req.query.ID)
    {
        res.status(400)
        res.send('ID is missing')
        return
    }

    try{
        let rows = getSubmissionStatement.all([req.query.ID])
        res.send(JSON.stringify(rows))
    }
    catch(e)
    {
        console.log('DB Error')
        console.log(e)

        res.status(500)
        res.send('DB Error')
        return
    }
})

const getAllSubmissionsStatement = db.prepare("SELECT * FROM Submissions");
api.get('/get_all_submissions', (req, res) => {
    try{
        let rows = getAllSubmissionsStatement.all()
        res.send(JSON.stringify(rows))
    }
    catch(e)
    {
        console.log('DB Error')
        console.log(e)

        res.status(500)
        res.send('DB Error')
        return
    }
})

const isSubmittedStatement = db.prepare(convertToSqlSelectIDQuery("Submissions", submitUniqueProperites, "LIMIT 1"))
api.post('/is_submitted', (req, res) => {
    const newSubmission = req.body

    let parameterList = convertObjectToArrayOfValues(submitUniqueProperites, newSubmission)

    try{
        let rows = isSubmittedStatement.all(parameterList);
        if (rows.length == 0)
            rows = 0
        else
            rows = rows[0].ID
        res.send(JSON.stringify(rows))
    }
    catch(e)
    {
        console.log('DB Error')
        console.log(e)

        res.status(500)
        res.send('DB Error')
        return
    }
})

const postSubmissionStatement = db.prepare(convertToSqlInsertQuery("Submissions", allProperties))
api.post('/post_submission', (req, res) => {
    const newSubmission = req.body
    if (!isObjectAllowedInDB(req.body))
    {
        res.status(400)
        res.send('Bad submission format')
        return
    }

    let isSubmittedParameterList = convertObjectToArrayOfValues(submitUniqueProperites, newSubmission)
    let parameterList = convertObjectToArrayOfValues(allProperties, newSubmission)

    try{
        let submitted = isSubmittedStatement.all(isSubmittedParameterList)
        if (submitted.length != 0)
        {
            res.status(400)
            res.send("Already submitted")
            return
        }

        let info = postSubmissionStatement.run(parameterList)

        console.log(`Inserted submission ID ${info.lastInsertRowid} - ${newSubmission["DXGI_ADAPTER_DESC.Description"]}`)
        res.send("" + info.lastInsertRowid)
        return
    }
    catch(e)
    {
        console.log('DB Error')
        console.log(e)

        res.status(500)
        res.send('DB Error')
        return
    }
})

if (process.env.SSLKey && process.env.SSLCert)
{
    console.log('Starting HTTPS Server')
    const options = {
        key: fs.readFileSync(process.env.SSLKey),
        cert: fs.readFileSync(process.env.SSLCert),
    }
    https.createServer(options, api).listen(apiPort, () => {
        console.log(`Started HTTPS Server on port ${apiPort}`)
    })
}
else
{
    console.log('Starting HTTP Server')
    http.createServer(api).listen(apiPort, () => {
        console.log(`Started HTTP Server on port ${apiPort}`)
    })
}
