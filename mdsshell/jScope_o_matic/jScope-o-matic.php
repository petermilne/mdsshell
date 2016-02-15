<html>
    <head>
           <title>jScope-o-matic $Revision: 1.6 $</title>
    </head>
	
    <body>
	<h1>Generate jScope configuration files</h1>
<p>Data model for D-TACQ cards in an MDSplus tree is typically in canonical form "CARD:CHxx".
<p>
</p>But configuring jScope by hand for large channel counts can be tedious - 
<b>jScope-o-matic</b> to the rescue!
</op>
<p>	Simply enter the parameters you want on the form below, press submit and the appropriate jScope configuration file is generated. Copy/Paste the output, save on your local machine in ~/jScope/configurations/myConfigFile, then run jScope with this file name as first argument.
</p>
<p>Examples:
<ul>
<li>One panel, many plots: set ROWS=1, COLS=1 NUMEXPR=numchannels
<li>One panel per plot:  set ROWS=N, COLS=1, NUMEXPR=1 - good to about 16 channels
<li>For very high channel count cards eg ACQ196, it's possible to show one panel per channel, but you need a very big screen :-) ROWS=16,COLS=8,NUMEXPR=1
<li>For 96 channels, our preferred display is ROWS=16,COLS=1,NUMEXPR=8
</ul>
<ul>
<li>SERVER is the logical name for the MDSIP server, SERVER_PORT is the DNS name (or ip address) + port number for the connection.
<li>OR select LOCAL so connect direct to a local MDSPLUS tree
</ul>
<ul>
<li>eg to connect direct to kilda and standard port : SERVER=kilda SERVER_PORT=kilda
<li>eg to connect to rhum via local SSH tunnel: SERVER=rhum SERVER_PORT="localhost:10001"
</ul>


	<?php 
		$prams = array(
			'TREE' => 'baobab',
			'SHOT' => '0',
			'ROWS'	 => '1',
			'COLS'	 => '1',
			'NUMEXPR' => '4',
			'FIELDFMT' => 'ACQ216_023:CH%02d',
			'EVENT'    => 'ACQ216_023',
			'SERVER'  => 'kilda',
			'SERVER_PORT' => 'kilda:8000');

                if(!empty($_POST['ROWS'])){
		       foreach ($prams as $k => $v){
		              $prams[$k] = $_POST[$k];
 		       }
                }
	?>
	<form action="<?php $_SERVER['PHP_SELF']; ?>" method="post">
	<table>
	<?php 
		foreach ($prams as $k => $v){
			echo "<tr><td>$k</td>";
			echo "<td><input type=\"text\" name=\"$k\" value=\"$v\"/>";
			echo "</td?</tr>";
		}
	?>
	<tr><td>
		<input type="radio" name="SERVER_CLASS" value="LocalDataProvider">Local</input>
		<input type="radio" name="SERVER_CLASS" value="MdsDataProvider">MDSIP</input>
	</td></tr>
	<tr><td>
		<input type="submit" value="Build the Script"/>
	</td></tr>
	</table>
	</form>
	<?php if(!empty($_POST['ROWS'])){
		$command = '';
		foreach ($_POST as $k => $v){
			$command .= "$k=\"$v\" ";
		}
		
		$command .= "./jscope_column.sh";
		echo "<pre>";
                echo "--------------------8<---------------------------";
		passthru("$command");
                echo "--------------------8<---------------------------";
		echo "</pre>";
	} ?>
    </body>
</html>


