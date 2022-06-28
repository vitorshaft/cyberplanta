/////////////// PRIMEIRA PARTE DA PAGINA

const char antes[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title>Cyber Planta</title>
<style>
body {color: #434343; font-family: "Helvetica Neue",Helvetica,Arial,sans-serif; font-size: 14px; background-color: #393939; margin-top: 100px;}
.container {margin: 0 auto; max-width: 400px; padding: 30px; box-shadow: 0 10px 20px rgba(0,0,0,0.19), 0 6px 6px rgba(0,0,0,0.23); background-color: #ffffff; border-radius: 10px;}
h2 {text-align: center; margin-bottom: 20px; margin-top: 0px; color: #0ee6b1; font-size: 35px;}
#titleGreen {color: #0ca060;}
#titleBlack {color: #4D4D4D;}
h3 {text-align: center; margin-bottom: 40px; margin-top: 0px; color: #0ca060; font-size: 20px;}
h4 {text-align: left; margin-bottom: 20px; margin-top: 0px; color: #4d4d4d; font-size: 16px;}
form .field-group {box-sizing: border-box; clear: both; padding: 4px 0; position: relative; margin: 1px 0; width: 100%;}
.text-field {font-size: 15px; margin-bottom: 4%; -webkit-appearance: none; display: block; background: #fafafa; color: #636363; width: 100%; padding: 15px 0px 15px 0px; text-indent: 10px; border-radius: 5px; border: 1px solid #e6e6e6; background-color: transparent;}
.text-field:focus {border-color: #00bcd4; outline: 0;}
.button-container {box-sizing: border-box; clear: both; margin: 1px 0 0; padding: 4px 0; position: relative; width: 100%;}
.button {background: #0ca060; border: none; border-radius: 5px; color: #ffffff; cursor: pointer; display: block; font-weight: bold; font-size: 16px; padding: 15px 0; text-align: center; text-transform: uppercase; width: 100%; -webkit-transition: background 250ms ease; -moz-transition: background 250ms ease; -o-transition: background 250ms ease; transition: background 250ms ease;}
p {text-align: center; text-decoration: none; color: #87c1d3; font-size: 18px;}
a {text-decoration: none; color: #ffffff; margin-top: 0%;}
#status {text-align: center; text-decoration: none; color: #336859; font-size: 14px;}
</style>
<script>
function statusPlanta() {
var intervalo = document.forms["myForm"]["intervalo"].value;
}
</script>
</head>
<body>
<div class="container">
<h2><span id="titleGreen">Cyber</span><span id="titleBlack">Planta &#x1F331</span></h2>
<h3>Como está sua planta agora?</h3>
<h4>
)=====";

////////////////////////ULTIMA PARTE DA PAGINA

const char depoisOn[] PROGMEM = R"=====(
</h4>
<form name="formTempo" action="/get">
<br>
<div class="field-group">
<input type="number" name="intervalo" maxlength="30" placeholder="&#9201 Mudar intervalo (h)"><br>
(Defina o intervalo de rega em horas)<br><br>
<div id="statusDiv">
<br>
</div>
<div class="button-container">
<a href=\"/timer/on\">
<input class="button" type="submit" value="Enviar">
</div>
</form>
</div>
</div>
</body>
</html>
)=====";

const char depoisOff[] PROGMEM = R"=====(
</h4>
<form name="formTempo" action="/get">
<br>
<div class="field-group">
<input type="number" name="intervalo" maxlength="30" placeholder="&#9201 Mudar intervalo (h)"><br>
(Defina o intervalo de rega em horas)<br><br>
<div id="statusDiv">
<br>
</div>
<div class="button-container">

<a href=\"/timer/off\">
<input class="button" type="submit" value="Enviar">
</a>
</div>
</form>
</div>
</div>
</body>
</html>
)=====";
