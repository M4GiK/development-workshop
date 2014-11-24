<?
	session_start();
	if($_GET["logout"] == "true")
	{
		//session_unregister("admin");
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

 <!--[if IE 7]>
        <link rel="stylesheet" type="text/css" href="../../frontend/style-css/style_ie7_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]-->
 
<meta http-equiv="content-language" content="cas" />
</head>

<body>

  <?
  
  
  if (isset($_POST["filtrar"]))
	{
		
		$filter["username"] = $_POST["username"];
		$filter["status"] = $_POST["status"];
		
		//fecha 
		if($_POST["mes_ini_fecha"]!=0 && $_POST["dia_ini_fecha"]!=0 && $_POST["year_ini_fecha"]!=0 && $_POST["mes_fin_fecha"]!=0 && $_POST["dia_fin_fecha"]!=0 && $_POST["year_fin_fecha"]!=0)
		{
			$filter["fecha"]["ini"] = mktime(0,0,0,$_POST["mes_ini_fecha"],$_POST["dia_ini_fecha"],$_POST["year_ini_fecha"]);
			$filter["fecha"]["fin"] = mktime(23,59,59,$_POST["mes_fin_fecha"],$_POST["dia_fin_fecha"],$_POST["year_fin_fecha"]);
		}
		
		$_SESSION["filter_exec"]["filter"] = $filter;
		$_SESSION["filter_exec"]["post"] = $_POST;
			
	}

	
  ?>
  
  <?
	include "../../management/executionManager.php";
	include "../../management/userManager.php";
	include "../../lib/Executor.php";
	include "../config.php"; 
	include "../../management/incidenceManager.php";
	include "../menus.php";
		
	
	function getHeader($default,$values)
	{
		echo "<tr>";
		if(isset($_GET["order"]) && isset($_GET["way"]))
		{
		 	$order = $_GET["order"];
		 	$way = $_GET["way"];
			
			if($way == "ASC")
			{	
				$img = "../../frontend/img/arrow_u.gif";				
				$waylink = "DESC";
			}	
			else
			{	
				$img = "../../frontend/img/arrow_rx.gif";
				$waylink = "ASC";
			}			
		}
		else
		{	
			$order = $default;
			$way = "ASC";
			$waylink = "DESC";			
			$img = "../../frontend/img/arrow_u.gif";
		}
		
		for($i = 0; $i < count($values); $i++)
		{
			if($order == $values[$i])
			{
				echo "<th class=\"th_tabladatos\" scope=\"col\"><strong><a href=\"index.php?order=$order&way=$waylink\">[$order] </a>		 <img src=\"$img\" alt=\"\" width=\"7\" height=\"7\" /></strong></th>";
			}
			else
			{
				echo "<th class=\"th_tabladatos\" scope=\"col\"><strong><a href=\"index.php?order=$values[$i]&way=ASC\">$values[$i]</a></strong></th>";			}	
		
		}
		echo "<th class=\"th_tabladatos\" scope=\"col\"><strong>parametros</strong></th>";			
		echo "<th class=\"th_tabladatos\" scope=\"col\"><strong>resultado</strong></th>";						
		echo "<th class=\"th_tabladatos\" scope=\"col\"><strong>acciones</strong></th>";			
		
		
		
		$info["order"] = $order;
		$info["way"] = $way;
		
		return $info;
		
	}		
	
	function getOptions($ini, $keys, $values, $selected)
	{
	
		if($selected == "0" || $selected == "")
			echo "<option value=\"0\" selected=\"selected\">$ini</option>";
		else
			echo "<option value=\"0\" >$ini</option>";
			
		for($i = 1; $i < count($keys); $i++)
		{
				if($values!= "")
				{
					if ($values[$i] != $selected)
						echo "<option value=\"$values[$i]\">$values[$i]</option>";
					else
						echo "<option value=\"$values[$i]\" selected=\"selected\">$values[$i]</option>";
				}
				else
				{
					if ($i != $selected)
						echo "<option value=\"$i\">$i</option>";
					else
						echo "<option value=\"$i\" selected=\"selected\">$i</option>";	
				}		
				
		}
	}
	
	function getOptionsYear($selected)
	{
		if($selected == "0" || $selected == "")
			echo "<option value=\"0\" selected=\"selected\">-</option>";
		else
			echo "<option value=\"0\" >-</option>";
			
		for($i = 7; $i <= 99; $i++)
		{
			$year = 2000 + $i;
			if ($year != $selected)
			{	
				if($i >= 10)
					echo "<option value=\"$year\">$i</option>";
				else
					echo "<option value=\"$year\">0$i</option>";	
			}	
			else
			{	
				if($i >= 10)
						echo "<option value=\"$year\" selected=\"selected\">$i</option>";
				else
					echo "<option value=\"$year\" selected=\"selected\">0$i</option>";
			}	
		}
	}
	
	function getOptionsUsername($selected)
	{
		$userm = new userManager(); 
		$users = $userm->getUsers("","");
		
		if($selected == "0" || $selected == "")
			echo "<option value=\"0\" selected=\"selected\">Cualquiera</option>";
		else
			echo "<option value=\"0\" >Cualquiera</option>";
			
		for($i = 0; $i < count($users); $i++)
		{
			$user = $users[$i]["username"];
			if ($user != $selected)
				echo "<option value=\"$user\">$user</option>";
			else
				echo "<option value=\"$user\" selected=\"selected\">$user</option>";
		}
								
	}
	
	function getExecutionsInfo($filter,$order,$page)
	{
		$exem = new executionManager();
		$exem->checkExecutionsStatus();
		$exec = $exem->getExecutions($filter, $order);
		
		if($page !="")
		{
			$i = ($page -1) * 8;
			$fin = $page * 8;
			
		}
		else
		{
			$i = 0;
			$fin = 8;
		}
		
		$pages = ceil(count($exec)/8);
		
		while ($i<$fin && $i < count($exec))
		{
			$username = $exec[$i]["username"];
			$time = $exec[$i]["fecha"];
			$fecha = date("G:i d/m/Y",$time);
			$file = "$username-$time";
			$parametros = $exec[$i]["parametros"];
			$status = $exec[$i]["status"];
			$condor_id = $exec[$i]["condor_id"];
			$trace = "$username-trace-$time";
			
			if($i % 2 != 0)
				$class= "td_tabladatos_IMPAR"; 
			else	
				$class= "td_tabladatos_PAR"; 
			echo "<tr>
				<td class=\"$class\">$username</td>
				<td class=\"$class\">$fecha</td>
				<td class=\"$class\">$status</td>
				<td class=\"$class\">$parametros</td>";
				
			if($status == "running")
				echo "<td class=\"$class\">running..</td>";
				
				
			else
				echo "<td class=\"$class\"><a href=\"../../frontend/result_admin.php?username=$username&date=$time\">Ver resultado</a></td>";
			
				
			echo "<td class=\"$class\"><a href=\"removeExecution.php?username=$username&fecha=$time\"> Eliminar</a></td>";
  			echo"</tr>";
			$i++;
		}
		
		return $pages;
	}
	
	function getPagination($pages,$actual,$order)
	{
		$order_by = $order["order"];
		$way = $order["way"];
		if($actual == "")
			$actual = 1;
		
		for($i = 1; $i <=$pages; $i++)
		{
			if($actual== $i)
				echo "<strong>$i</strong>&nbsp;";
			else
				echo "<a href=\"index.php?page=$i&order=$order_by&way=$way\">$i</a>&nbsp;";	
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
			<? getTabsGestion("Ejecuciones",$num);?>
		</ul>
	</div>
	<div id="parte_central">
	<div id="menu_interior">
		<ul class="ul_segundomenu">
			<li class="li_segundomenu"><span class="segundomenu_selec">Ver ejecuciones</span></li>
			<li class="li_segundomenu"><a href="stats.php">Ver estad&iacute;sticas</a></li>
			<li class="li_segundomenu"><a href="params.php">Parametros</a></li>
		</ul>
	</div>	 
<div id="result" align="center">
<h1 align="center">Ejecuciones</h1>
<form id="form1" method="post" action="index.php<? 
if(isset($_GET["order"]) && isset($_GET["way"]))
	echo "?order=".$_GET["order"]."&way=".$_GET["way"];				
?>">
  <table width="750" cellspacing="0" >
    <tr>
      <td width="171" bgcolor="#EFEFEF">filtrar por status </td>
      <td width="569" align="left"><select name="status" id="status">
        <?
			getOptions("Cualquiera",array("","finished","running"),array("","finished","running"),$_SESSION["filter_exec"]["post"]["status"]);	
		?>
      </select>      </td>
    </tr>
    <tr>
      <td>filtrar por fecha : </td>
      <td valign="middle" align="left"><select name="dia_ini_fecha" id="dia_ini_fecha">
        <? getOptions("-",array_fill(0,32,0),"",$_SESSION["filter_exec"]["post"]["dia_ini_fecha"]);?>
      </select>
        /
        <select name="mes_ini_fecha" id="mes_ini_fecha">
          <? getOptions("-",array_fill(0,13,0),"",$_SESSION["filter_exec"]["post"]["mes_ini_fecha"]);?>
        </select>
        /
        
        20
        <select name="year_ini_fecha" id="year_ini_fecha" >
          <? getOptionsYear($_SESSION["filter_exec"]["post"]["year_ini_fecha"]);?>
        </select>
        entre
        <select name="dia_fin_fecha" id="dia_fin_fecha">
          <? getOptions("-",array_fill(0,32,0),"",$_SESSION["filter_exec"]["post"]["dia_fin_fecha"]);?>
        </select>
        /
        <select name="mes_fin_fecha" id="mes_fin_fecha">
          <? getOptions("-",array_fill(0,13,0),"",$_SESSION["filter_exec"]["post"]["mes_fin_fecha"]);?>
        </select>
        /
        20
        <select name="year_fin_fecha" id="year_fin_fecha" >
          <? getOptionsYear($_SESSION["filter_exec"]["post"]["year_fin_fecha"]);?>
        </select></td>
    </tr>
    <tr>
      <td bgcolor="#EFEFEF" >filtrar por usuario</td>
      <td align="left"><select name="username" id="username">
        <? getOptionsUsername($_SESSION["filter_exec"]["post"]["username"]);?>
      </select></td>
    </tr>
	<tr>
		<td colspan="2">&nbsp;</td>
	</tr>
    <tr>
      <td colspan="2"><input name="filtrar" type="submit" id="filtrar" value="aplicar filtro" /></td>
    </tr>
  </table>
  
</form><br />

<table width="80%" cellpadding="0" cellspacing="0" class="tabladatos">
  <? 
  	$order = getHeader("username",array("username","fecha","status"));	
	$pages = getExecutionsInfo($_SESSION["filter_exec"]["filter"], $order,$_GET["page"]);
  ?>
  <tr>
  <td colspan="6" class="td_paginacion"><? getPagination($pages,$_GET["page"],$order); ?>
    </td>
  </tr>
   <tr class="tr_tabladatosfin">
      <td></td>
      <td></td>
      <td></td>
      <td></td>
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
