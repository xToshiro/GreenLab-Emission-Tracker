// Função para criar um gráfico
const createChart = (renderTo, titleText, yAxisText) => new Highcharts.Chart({
  chart: { renderTo },
  title: { text: titleText },
  series: [{ showInLegend: false, data: [] }],
  plotOptions: {
    line: { animation: false, dataLabels: { enabled: true } },
    series: { color: '#059e8a' }
  },
  xAxis: { type: 'datetime', dateTimeLabelFormats: { second: '%H:%M:%S' } },
  yAxis: { title: { text: yAxisText } },
  credits: { enabled: false }
});

// Função para atualizar um gráfico
const updateChart = (chart, url) => {
  setInterval(() => {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        var x = (new Date()).getTime(),
            y = parseFloat(this.responseText);
        if(chart.series[0].data.length > 25) {
          chart.series[0].addPoint([x, y], true, true, true);
        } else {
          chart.series[0].addPoint([x, y], true, false, true);
        }
      }
    };
    xhttp.open("GET", url, true);
    xhttp.send();
  }, 2500);
};

// Criação dos gráficos
const chartT = createChart('chart-temperature', 'Temperatura °C', 'Temperatura (Celsius)');
const chartH = createChart('chart-humidity', 'Umidade %', 'Umidade (%)');
const chartPRESS = createChart('chart-pressure', 'Pressao (mmHg)', 'Pressao (mmHg)');
const chartPMONE = createChart('chart-pm1', 'PM 1', '(ug/m3)');
const chartPMSEC = createChart('chart-pm25', 'PM 2.5', '(ug/m3)');
const chartPMTEN = createChart('chart-pm10', 'PM 10', '(ug/m3)');
const chartOZON = createChart('chart-ozon', 'Ozonio', '(ppm)');
const chartCO = createChart('chart-co', 'CO2', '(ppm)');
const chartNOX = createChart('chart-nox', 'NOX', '(ppm)');

// Atualização dos gráficos
updateChart(chartT, '/temp');
updateChart(chartH, '/humidity');
updateChart(chartPRESS, '/press');
updateChart(chartPMONE, '/pm1');
updateChart(chartPMSEC, '/pm25');
updateChart(chartPMTEN, '/pm10');
updateChart(chartOZON, '/ozon');
updateChart(chartCO, '/co');
updateChart(chartNOX, '/nox');

// Funções para trocar o modo (escuro/claro)
/* const toggleDarkMode = () => document.body.className = "dark-mode";
const toggleLightMode = () => document.body.className = "light-mode"; */

