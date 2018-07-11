const request = require('request');
const fs = require('fs');
var data = {};
 
try{
    data = JSON.parse(fs.readFileSync("./data.json",'utf8'));
}
catch(e){}

request('http://grow.lhclab.com', { json: true }, (err, res, body) => {
    if (err) { 
        return console.log(err); 
    }
    else{
        try{
            const curTime = Math.floor(new Date() / 1000);
            data[curTime]= {
                time       : curTime,
                waterTemp  : body.temp[0],
                airTemp    : body.airTemp,
                vrmTemp    : body.temp[1],
                humidity   : body.airHumidity,
                light      : body.light
            };

            const maxHold = 100;
            var keys = Object.keys(data);
            if(keys.length > maxHold){
                for(var i=0 ; i < (keys.length - maxHold) ; i++){
                    delete data[keys[i]];
                }
            }
            
            fs.writeFile('./data.json', JSON.stringify(data,null, 2), function (err) {
            if (err) throw err;
                console.log(JSON.stringify(data[curTime], null, 2));
            });
        }
        catch(e){}
    }
});