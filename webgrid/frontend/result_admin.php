<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Web GRID</title>
<link href="style-css/ejecucion.css" rel="stylesheet" type="text/css" />
 <!--[if IE 6]>
        <link rel="stylesheet" type="text/css" href="style-css/style_ie6_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]--> 
 <!--[if IE 7]>
        <link rel="stylesheet" type="text/css" href="style-css/style_ie7_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]-->
 
<meta http-equiv="content-language" content="cas" />
</head>
<body>

<?
	include "../admin/config.php";
	include "../lib/File.php";
	include "../management/configManager.php";
	
	function getTabs($seccion)
	{
		if($seccion == "result" || $seccion =="")
			echo "<li class=\"pestana_selec\"><strong>Result</strong></li>";
		else
			echo "<li class=\"pestanas\"><a href=result_admin.php?username=".$_GET["username"]."&date=".$_GET["date"]."&section=result>Result</a></li>";	
			
		if($seccion == "stats_result")
			echo "<li class=\"pestana_selec\"><strong>Result stats</strong></li>";
		else
			echo "<li class=\"pestanas\"><a href=result_admin.php?username=".$_GET["username"]."&date=".$_GET["date"]."&section=stats_result>Result stats</a></li>";		
			
		if($seccion == "trace")
			echo "<li class=\"pestana_selec\"><strong>Trace</strong></li>";
		else
			echo "<li class=\"pestanas\"><a href=result_admin.php?username=".$_GET["username"]."&date=".$_GET["date"]."&section=trace>Trace</a></li>";	
		
		if($seccion == "stats_trace")
			echo "<li class=\"pestana_selec\"><strong>Trace stats</strong></li>";
		else
			echo "<li class=\"pestanas\"><a href=result_admin.php?username=".$_GET["username"]."&date=".$_GET["date"]."&section=stats_trace>Trace stats</a></li>";			
	}
	
	function getMenuInterior($seccion, $username, $date)
	{
		if($seccion =="" || $seccion =="trace" || $seccion =="result")
		{
			
			
			if($seccion =="" || $seccion == "result")
			{
				$file = "$username-$date.txt";
				$linkXLS = "../lib/export.php?file=$file&type=result&export=xls";
				$linkPDF = "../lib/export.php?file=$file&type=result&export=pdf";
			}
			
			else
			{	
				$file = "$username-trace-$date.txt";
				$linkXLS = "../lib/export.php?file=$file&type=trace&export=xls";
				$linkPDF = "../lib/export.php?file=$file&type=trace&export=pdf";
				
			}	
			echo "<li class=\"li_segundomenu\"><a href=\"$linkPDF\">Export as PDF</a></li>";
			echo "<li class=\"li_segundomenu\"><a href=\"$linkXLS\">Export as XLS</a></li>";
		} 
	}
	
	function getResult($username, $date)
	{
		$file = "$username-$date.txt";
		$output = OUTPATH."/$file";
		$file = new File($output, "result");
		echo "<h1>Result</h1>";
		echo $file->getHTML("",true);
		
	}
	function getStatsResult($username, $date)
	{
		$output = OUTPATH."/$username-$date.txt";
		$file = new File($output, "result");
		$parsed = $file->parse();
		echo "<h1>Result stats</h1>";	
		echo "<p><strong>Time</strong></p>";
		if($parsed["# Runs"] > 1)
			echo "<img src=\"resultStats.php?graph=runs_time&username=$username&date=$date\" width=\"350\" height=\"200\"/>";
		
		else
		{
			echo "<img src=\"resultStats.php?graph=time1&username=$username&date=$date\" width=\"350\" height=\"200\"/>";
			echo "<img src=\"resultStats.php?graph=time2&username=$username&date=$date\" width=\"350\" height=\"200\"/>";
		
		}
		
		echo "<p><strong>Host</strong></p>";
		if($parsed["# Runs"] > 1)
			echo "<img src=\"resultStats.php?graph=runs_hosts&username=$username&date=$date\" width=\"350\" height=\"200\"/>";

		else
		{
			echo "<img src=\"resultStats.php?graph=host_utilization&username=$username&date=$date\" width=\"350\" height=\"200\"/>";
			echo "<img src=\"resultStats.php?graph=number_hosts&username=$username&date=$date\" width=\"350\" height=\"200\"/>";
		}
		
		if($parsed["# Runs"] == 1)
		{
			echo "<p><strong>Queue & wait</strong></p>";
			echo "<img src=\"resultStats.php?graph=queue&username=$username&date=$date\" width=\"350\" height=\"200\"/>";
			echo "<img src=\"resultStats.php?graph=wait&username=$username&date=$date\" width=\"350\" height=\"200\"/>";
			echo "<p><strong>Activations & schedules</strong></p>";
			echo "<img src=\"resultStats.php?graph=schedules&username=$username&date=$date\" width=\"350\" height=\"200\"/>";
			echo "<img src=\"resultStats.php?graph=activations&username=$username&date=$date\" width=\"350\" height=\"200\"/>";
		}	
	}
	function getTrace($username,$date, $trace)
	{
		$file = "$username-trace-$date.txt";
		
		$output = OUTPATH."/$file";
		$file = new File($output, "trace");
		echo "<h1>Trace</h1>";
		if($trace == "")
			$result = $file->getHTML(0,true);
		else
			$result = $file->getHTML($trace,true);	
		
		echo "<form id=\"form1\" name=\"form1\" method=\"get\" action=\"result_admin.php\">";
		echo "<table width=\"200\" cellspacing=\"0\">
  				<tr>
    			<td bgcolor=\"#EFEFEF\" align=\"center\"><strong>Select Trace</strong></td>";
    	echo "<td><select name=\"trace\">";
  		
		for	($i = 1; $i<=$result["total"]; $i++)
		{
			$index = $i - 1;
			if($index == $trace)
				echo "<option value=\"$index\" selected=\"selected\">$i</option>";
			else
				echo "<option value=\"$index\">$i</option>";
		}	
		
  		echo "</select>
  				<input type=\"hidden\" name =\"username\" value=\"$username\"/>
				<input type=\"hidden\" name =\"date\" value=\"$date\"/>
				<input type=\"hidden\" name =\"section\" value=\"trace\"/>
				</td></tr>
				<tr><td colspan=\"2\" align=\"center\"><input type=\"submit\" name=\"Submit\" value=\"View trace\" /></td></tr></table>
		</form><br/>";
		echo $result["html"];
	}
	
	function getStatsTrace($username, $date,$trace)
	{			
		$file = "$username-trace-$date.txt";
		$output = OUTPATH."/$file";
		$file = new File($output, "trace");
		$result = $file->parse();	
		
		if($trace == "")
			$trace = 0;
		
		$index = $trace + 1;
		echo "<h1>Trace stats  $index / ".count($result)."</h1>";
		echo "<form id=\"form1\" name=\"form1\" method=\"get\" action=\"result_admin.php\">";
		echo "<table width=\"200\" cellspacing=\"0\">
  				<tr>
    			<td bgcolor=\"#EFEFEF\" align=\"center\"><strong>Select Trace</strong></td>";
  		echo "<td><select name=\"trace\">";
		
		for	($i = 1; $i<=count($result); $i++)
		{
			$index = $i - 1;
			if($index == $trace)
				echo "<option value=\"$index\" selected=\"selected\">$i</option>";
			else
				echo "<option value=\"$index\">$i</option>";
		}	
		
  		echo "</select>
  				<input type=\"hidden\" name =\"username\" value=\"$username\"/>
				<input type=\"hidden\" name =\"date\" value=\"$date\"/>
				<input type=\"hidden\" name =\"section\" value=\"stats_trace\"/>
				</td></tr>
				<tr><td colspan=\"2\" align=\"center\"><input type=\"submit\" name=\"Submit\" value=\"View trace\" /></td></tr></table>
		</form><br/>";
	
		echo "<img src=\"resultStats.php?graph=timeline_trace&username=$username&date=$date&trace=$trace\" width=\"800\" height=\"200\"/>";		echo "<img src=\"resultStats.php?graph=host_trace&username=$username&date=$date&trace=$trace\" width=\"800\" height=\"200\"/>";
		echo "<img src=\"resultStats.php?graph=task_trace&username=$username&date=$date&trace=$trace\" width=\"800\" height=\"600\"/>";
	}
	
	$cm =  new configManager();
	$message = $cm->getMessageAdmin()	
	
?>

<div id="contenido">
	<div id="cabecera">
		<div id="logo"><img src="img/resultadoejecucion.gif" alt="Resultados ejecucion" /></div>
	</div>
	<div id="central">
		<div id="primer_menu">
			<ul class="ul_primermenu">
				<li class="li_primermenu"><a href="../index.php?section=1" class="link_primermenu">Home</a></li>
				<li class="li_primermenu"><a href="../admin/executions/index.php" class="link_primermenu">back</a></li>
			</ul>
		</div>	
		<div id="destacado_gris">
			<ul>
				<li class="li_datos"><strong>User</strong> <? echo $_GET["username"];?></li>
				<li class="li_datos"><strong>Date</strong> <? echo date("G:i d/m/Y",$_GET["date"]); ?></li>
				<li class="li_datos"><strong><? echo $message;?></strong></li>
			</ul>
			
		</div>
	
	<div id="pestanas"> 
		<ul class="ul_pestana">
			<? getTabs($_GET["section"]);?>
		</ul>
	</div>
	<div id="parte_central">
	<div id="menu_interior">
		<ul class="ul_segundomenu">
			<? getMenuInterior($_GET["section"],$_GET["username"],$_GET["date"]);?>
			
		</ul>
	</div>	 
<div id="result" align="center">
<? 
	if(isset($_GET["username"]) && isset($_GET["date"]))
	{
		
		switch($_GET["section"])
		{
			case "":
			case "result":	
							getResult($_GET["username"],$_GET["date"]);
							break;
			case "trace":
							getTrace($_GET["username"],$_GET["date"],$_GET["trace"]);
							break;
			case "stats_result":
								getStatsResult($_GET["username"],$_GET["date"]);
								break;
			case "stats_trace":
								getStatsTrace($_GET["username"],$_GET["date"],$_GET["trace"]);
								break;													
		}
	}		
?>
</div>

<br />
<br />

	</div>
</div>
<div id="peu">&nbsp;</div>
</div>	
</body>
</html>
