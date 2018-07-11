function Unix_timestamp(t)
{
    var dt = new Date(t*1000);
    var hr = dt.getHours();
    var m = "0" + dt.getMinutes();
    var s = "0" + dt.getSeconds();
    return hr+ ':' + m.substr(-2) + ':' + s.substr(-2);  
}


$(document).ready(function(){ 
    $.getJSON( "data.json", function( data ) {
        var ctx = document.getElementById("chart");
        var chartDataAirTemp = [];
        var chartDataWaterTemp = [];
        var chartDataVRMTemp = [];
        var chartDataHumidity = [];
        var chartLabels = [];
    
        for(var key in data){
            chartDataAirTemp.push(data[key].airTemp);
            chartDataWaterTemp.push(data[key].waterTemp)
            chartDataVRMTemp.push(data[key].vrmTemp);
            chartDataHumidity.push(data[key].humidity);
            chartLabels.push(Unix_timestamp(parseInt(key)));
        }

        var chartParams = {
            type: 'line',
            data : {
                labels : chartLabels,
                datasets: [{
                    label : "airTemp",
                    borderColor : "#6f8cd2",
                    fill: false,
                    data: chartDataAirTemp
                },
                {
                    label : "waterTemp",
                    fill: false,
                    borderColor : "#578c49",
                    data: chartDataWaterTemp
                },
                {
                    label : "VRMTemp",
                    fill: false,
                    borderColor : "#ec6500",
                    data: chartDataVRMTemp
                },
                {
                    label : "humdity",
                    fill: false,
                    borderColor : "#243151",
                    data: chartDataHumidity
                },
                ]
            },
            options: {
                title: {
                    display: false,
                    text: 'grow'
                  }
            }
        };

        console.log(chartParams);

        var myLineChart = new Chart(ctx,chartParams );
    });
}); 