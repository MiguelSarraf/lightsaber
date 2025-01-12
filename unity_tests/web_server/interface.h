#ifndef INTERFACE_H
#define INTERFACE_H

const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
  <title>My lighsaber configuration page</title>
  <style>
    body {color: #434343; font-family: "Helvetica Neue",Helvetica,Arial,sans-serif; font-size: 14px; background-color: #eeeeee; margin-top: 100px;}
    .container {margin: 0 auto; max-width: 400px; padding: 30px; box-shadow: 0 10px 20px rgba(0,0,0,0.19), 0 6px 6px rgba(0,0,0,0.23); background-color: #ffffff; border-radius: 10px;}
  h2 {text-align: center; margin-bottom: 20px; margin-top: 0px; color: #0ee6b1; font-size: 35px;}
  #titleGreen {color: #00E1AA;}
  #titleBlack {color: #000000;}
    h3 {text-align: center; margin-bottom: 40px; margin-top: 0px; color: #336859; font-size: 35px;}
    form .field-group {box-sizing: border-box; clear: both; padding: 4px 0; position: relative; margin: 1px 0; width: 100%;}
    .text-field {font-size: 15px; margin-bottom: 4%; -webkit-appearance: none; display: block; background: #fafafa; color: #636363; width: 100%; padding: 15px 0px 15px 0px; text-indent: 10px; border-radius: 5px; border: 1px solid #e6e6e6; background-color: transparent;}
    .text-field:focus {border-color: #00bcd4; outline: 0;}
    .button-container {box-sizing: border-box; clear: both; margin: 1px 0 0; padding: 4px 0; position: relative; width: 100%;}
    .button {background: #00E1AA; border: none; border-radius: 5px; color: #ffffff; cursor: pointer; display: block; font-weight: bold; font-size: 16px; padding: 15px 0; text-align: center; text-transform: uppercase; width: 100%; -webkit-transition: background 250ms ease; -moz-transition: background 250ms ease; -o-transition: background 250ms ease; transition: background 250ms ease;}
    .column {
      float: left;
      width: 50%;
    }

    /* Clear floats after the columns */
    .row:after {
      content: "";
      display: table;
      clear: both;
    }
    p {text-decoration: none; color: #87c1d3;}
    pre {text-decoration: none; color: #87c1d3;}
    a {text-decoration: none; color: #ffffff; margin-top: 0%;}
    #status {text-align: center; text-decoration: none; color: #336859; font-size: 14px;}
  </style>
</head>
<body>
<div class="container">
  <div>
    <div class="column">
      <p align="left" stye="font-size:6;">ID : [id_placeholder]</p>
    </div>
    <div class="column">
      <pre align="right" stye="font-size:10;">[battery_placeholder]% [battery_bars_placeholder]</pre>
    </div>
  </div>
  <h2><span id="titleBlack">My</span><span id="titleGreen">Lightsaber</span></h2>
  <form name="myForm" action="/save_configurations" method="post">
    <div class="field-group">
  <label>Blade color</label>
  <input class='color' type='color' name='blade_color' value='[color_placeholder]'>
    </div>
    <br>
    <div class="field-group">
    <label>Activation speed</label>
    <input class="range" type="range" name="speed" min="1" max="100" value="[speed_placeholder]">
    </div>
  <br>
  <div id="statusDiv">
    <br><br>
  </div>
    <div class="button-container">
    <input class="button" type="submit" value="Save configurations">
    </div>
  </form>
</div>
</body>
</html>
)=====";

#endif