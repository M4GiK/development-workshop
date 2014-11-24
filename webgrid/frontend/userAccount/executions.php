<?
	session_start();
	if($_GET["logout"] == "true")
	{
		session_unregister("login");
		header("location:../../index.php");
		exit();
	}
	else
	{
		//session_register("login");
		if(!isset($_SESSION["login"]))
			header("location:login.php");
	}		
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Web GRID</title>
<link href="../style-css/style_admin_usuario.css" rel="stylesheet" type="text/css" />
 <!--[if IE 6]>
        <link rel="stylesheet" type="text/css" href="../style-css/style_ie6_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]--> 
 <!--[if IE 7]>
        <link rel="stylesheet" type="text/css" href="../style-css/style_ie7_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]-->
 
<meta http-equiv="content-language" content="cas" />
</head>
<body>
<?
	include "../../management/executionManager.php";
	include "../../lib/Executor.php";
	include "../../admin/config.php";
	
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
				echo "<a href=\"executions.php?page=$i&order=$order_by&way=$way\">$i</a>&nbsp;";	
		}
	}
	function showExecutions($exec,$page)
	{
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
			if($i % 2 != 0)
				$class= "td_tabladatos_IMPAR"; 
			else	
				$class= "td_tabladatos_PAR"; 
				
			$username = $exec[$i]["username"];
			$time = $exec[$i]["fecha"];
			$fecha = date("G:i d/m/Y",$time);
			$file = "$username-$time";
			$parametros = $exec[$i]["parametros"];
			$status = $exec[$i]["status"];
			$condor_id = $exec[$i]["condor_id"];
			$trace = "$username-trace-$time";
			
			echo "<tr>
			<td class=\"$class\">$fecha</td>
			<td class=\"$class\">$parametros</td>
			<td class=\"$class\">$status</td>";
			
			if($status == "running")
				echo "<td class=\"$class\">running..</td>";

			else
				echo "<td class=\"$class\"><a href=\"../result.php?username=$username&date=$time\">View result</a></td>";

				
		  	echo"</tr>";
			
			$i++;
		}
		
		return $pages;
	}
	
	$exem = new executionManager();
	$exem->checkUserExecutionsStatus($_SESSION["login"]["username"]);
	$exec = $exem->getUserExecutions($_SESSION["login"]["username"]);
?>


<div id="contenido">
	<div id="cabecera">
		<div id="logo"><img src="../img/cuenta_usuario.gif" alt="Cuenta Usuario" /></div>
	</div>
	<div id="central">
		<div id="primer_menu">
			<ul class="ul_primermenu">
				<li class="li_primermenu"><a href="../../index.php?section=1" class="link_primermenu">Home</a></li>
				<li class="li_primermenu"><a href="executions.php?logout=true" class="link_primermenu">Logout</a></li>
			</ul>
		</div>	
		<div id="destacado_gris">
			<ul>
				<li class="li_datos"><strong><? echo $_SESSION["login"]["username"];?></strong></li>
				<li class="li_datos"><strong> Mail:</strong> <? echo $_SESSION["login"]["mail"]; ?></li>
				<li class="li_datos"><strong> Institution:</strong><? echo $_SESSION["login"]["institucion"];?></li>
				<li class="li_datos"><strong> Last login:</strong><? echo date("G:i d/m/Y",$_SESSION["login"]["ultimo_login"]);?></li>
			</ul>
		</div>
	
	<div id="pestanas"> 
		<ul class="ul_pestana">
			<li class="pestana_selec"><strong>Executions</strong></li>
			<li class="pestanas"><a href="incidences.php">Incidences</a></li>
			<li class="pestanas"><a href="preferences.php">Settings</a></strong></li>
		</ul>
	</div>
	<div id="parte_central">
	<div id="menu_interior">
		<ul class="ul_segundomenu">
			<li class="li_segundomenu"><span class="segundomenu_selec">My executions</span></li>
			<li class="li_segundomenu"><a href="params.php">New execution</a></li>
		</ul>
	</div>	 
<div id="result" align="center">
<h1> My executions</h1>
<table width="80%" cellpadding="0" cellspacing="0" class="tabladatos">
  <tr>
    <th class="th_tabladatos"><strong>date</strong></th>
    <th class="th_tabladatos"><strong>parameters</strong></th>
    <th class="th_tabladatos"><strong>status</strong></th>
    <th class="th_tabladatos"><strong>result</strong></th>
  </tr>
  <? $pages = showExecutions($exec,$_GET["page"]);?>
  <tr>
  <td colspan="4" class="td_paginacion"><? getPagination($pages,$_GET["page"],""); ?>
    </td>
  </tr>
   <tr class="tr_tabladatosfin">
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
