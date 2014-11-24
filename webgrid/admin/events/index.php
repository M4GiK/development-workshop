<?
	session_start();
	if($_GET["logout"] == "true")
	{
		session_unregister("admin");
		header("location:../../index.php");
		exit();
	}
	else
	{
		//session_register("admin");
		if(!isset($_SESSION["admin"]))
			header("location:../login.php");
	}	
	
	
?>	
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Web GRID</title>
<link href="../../frontend/style-css/style_admin_usuario.css" rel="stylesheet" type="text/css" />
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
	include "../config.php";
	include "../../management/eventManager.php";
	include "../../management/incidenceManager.php";
	include "../menus.php";
	
	function getInterval($event)
	{
		$interval = true;
		$ciclic_day = false;
		$ciclic_month = false;
		$ciclic_year = false;
		
		if($event["dia"] == $event["dia_fin"] && $event["mes"] == $event["mes_fin"] && $event["year"] == $event["year_fin"])
			$interval = false;
		
		if($event["dia"] == 0)
			$ciclic_day =true; 
		if($event["mes"] == 0)
			$ciclic_month = true;	
		if($event["year"] == 0)
			$ciclic_year = true;
		
		if($ciclic_day == false)
			$str = $event["dia"];
		else
			$str = "todos los dias";
		
		if($ciclic_month == false)
			$str .= " / ".$event["mes"];
		else					
			$str .= " / todos los meses ";
		
		if($ciclic_year == false)
			$str .= " / ".$event["year"];
		else
			$str .= " / todos los a&ntilde;os";
			
		if($interval == true)
		{
			$str.=" - ".$event["dia_fin"]." / ".$event["mes_fin"]." / ".$event["year_fin"];		
		}
		
		return $str;	 		
	}
	function addRowEVent($event,$class)
	{
		$titular = $event["titulo_es"];
		$intervalo = getInterval($event);
		$id = $event["id_evento"];
		echo "
				<tr>
    				<td class=\"$class\">$titular</td>
					<td class=\"$class\">$intervalo</td>
    				<td class=\"$class\"><a href=\"manageEvent.php?action=edit&event=$id\">modificar</a> <a href=\"removeEvent.php?event=$id\">eliminar</a> </td>
				</tr>";
	}
	
	function getEventsInfo()
	{
		$em = new eventManager();
		$events = $em->getEvents();
		
		for($i = 0; $i < count($events); $i++)
		{
			if($i % 2 != 0)
				$class= "td_tabladatos_IMPAR"; 
			else	
				$class= "td_tabladatos_PAR"; 
				
			addRowEvent($events[$i],$class);
		}
	}
	
		$im = new incidenceManager();
		$num = $im->getNumberWaitingIncidences();

?>
<div id="contenido">
	<div id="cabecera">
		<div id="logo"><? include "../header.htm"?></div>
	</div>
	<div id="central">
		<div id="primer_menu">
			<ul class="ul_primermenu">
				<? getFirstMenuGestion();?>
			</ul>
		</div>	
		
	<div id="pestanas"> 
		<ul class="ul_pestana">
			<? getTabsGestion("Eventos",$num);?>
		</ul>
	</div>
	<div id="parte_central">
<div id="result" align="center">
<h1 align="center">Eventos<br/></h1>
<table width="80%" cellpadding="0" cellspacing="0" class="tabladatos">
  <tr>
    <th class="th_tabladatos"><strong>Evento</strong></th>
	<th class="th_tabladatos"><strong>Intervalo</strong></th>
    <th class="th_tabladatos"><strong>Ajustes</strong></th>
  </tr>
 <?
 	getEventsInfo();
 ?>
  <tr>
    <td colspan="3" class="td_tabladatos_PAR">&nbsp;</td>
  </tr>
  <tr>
    <td colspan="3" class="td_tabladatos_PAR"><a href="manageEvent.php?action=new"> A&ntilde;adir evento</a></td>
  </tr>
  <tr class="tr_tabladatosfin">
      <td></td>
      <td></td>
      <td></td>
    </tr>
  
</table>
</div>
<br />
<br />

	</div>
</div>
<div id="peu">&nbsp;</div>
</div>
</body>
</html>
