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
<?
	
	include "../config.php";
	include "../../management/eventManager.php";
	
	function showErrors($errors)
	{
		for($i = 0; $i < count($errors); $i++)
		{
			echo "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;".$errors[$i]."<p/>";
		}	
	}
	function checkEvent($params)
	{
		$errors = array();
		if($params["year"] == "" || $params["year_fin"]=="" || !is_numeric($params["year"]) || !is_numeric($params["year_fin"]))
			$errors[] = "Debe de introducir un a&ntilde;o tanto en la fecha de inicio como en la fecha fin";
		
		foreach( $params as $key => $value)
		{
			if($value == "0")
			{
				switch($key)
				{
					case "dia":
					case "dia_fin":
					case "mes":
					case "mes_fin":
									$params[$key] = 1;
									break;
									
					case "year":
					case "year_fin": 
									$params[$key] = 2007;
									break;
				}
			}
		}
		
		$date_ini = checkdate($params["mes"],$params["dia"],$params["year"]);
		if($date_ini === false)
			$errors[] = "La fecha inicial es incorrecta";
		
		if(!checkdate($params["mes_fin"],$params["dia_fin"],$params["year_fin"]))
			$errors[] = "La fecha final es incorrecta";
		
		if(mktime(0,0,0,$params["mes"],$params["dia"],$params["year"]) > mktime(0,0,0,$params["mes_fin"],$params["dia_fin"],$params["year_fin"]))
			$errors[] = "Intervalo incorrecto"; 
			
		return $errors;			
	}
	
	if($_POST["Submit"]!= "")
	{
		$event = $_POST["event"];
		$action  = $_POST["action"];
		$info = $_POST;
			
		$errors = checkEvent($_POST);
		
		if(count($errors) == 0)
		{
			$em = new eventManager();
			switch($action)
			{
				case "edit":	
							if($em->editEvent($_POST,$event))
									header("location:messageEvent.php?message=edit");
							else
								$errors[] = "No se ha podido modificar";
			
							break;
							
				case "new":
							if($em->addEvent($_POST))
								header("location:messageEvent.php?message=new");
							else
								$errors[] = "No se ha podido a&ntilde;adir el evento";
				
							break;				
			}
		}
		
		//echo "<p align=\"center\"><img src=\"../../frontend/img/icono_amarill_square.gif\"/><a href=\"index.php\">&nbsp;volver</a></p>";
	}
	else
	{
		$action = $_GET["action"];
		$event = $_GET["event"];
	
		
		$em = new eventManager();
		switch($action)
		{
			case "edit":
						//echo "section:$section subsection:$subsection<br/>";
						$info = $em->getEvent($event);
						//print_r($info);
						
						break;
		}
	}		
?>
<?

	include "../../management/incidenceManager.php";
	include "../menus.php";
	
	
	$im = new incidenceManager();
		$num = $im->getNumberWaitingIncidences();
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
	<div id="menu_interior">
		<ul class="ul_segundomenu">
			<li class="li_segundomenu"><a href="index.php">Volver</a></li>
		</ul>
	</div>	 
<div id="result" align="center">
<h1 align="center">Editar Eventos<br/></h1>
<div id="error">
	<? showErrors($errors);?>
</div>
<form action="manageEvent.php" method="post" name="form1" id="form1">
  <table width="80%" cellpadding="0" cellspacing="0" class="tabladatos">
    <tr>
      <th class="th_tabladatos" scope="row"><strong>titulo_es</strong></td>
      <td class="td_tabladatos_PAR"><input name="titulo_es" type="text" id="titulo_es" value="<? echo $info["titulo_es"];?>" /> 
      <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>titulo_en</strong></td>
      <td class="td_tabladatos_PAR"><input name="titulo_en" type="text" id="titulo_en" value="<? echo $info["titulo_en"]; ?>" />
      <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>titulo_ca</strong></td>
      <td class="td_tabladatos_PAR"><input name="titulo_ca" type="text" id="titulo_ca" value="<? echo $info["titulo_ca"];?>"/>
          <br /></td>
    </tr>
	 <tr>
      <th class="th_tabladatos" scope="row"><strong>intervalo </strong></td>
      <td class="td_tabladatos_PAR">
	  <p><strong>Nota:</strong>Para introducir un a&ntilde;o c&iacute;clico debe poner 0 en el campo correspondiente </p>
	  Dia: 
        <select name="dia" size="1" id="dia">
        <?
		   		$daysel = $info["dia"];
				
				if($daysel == 0)
					echo "<option value=\"0\" selected=\"selected\" >Ciclico</option>";
				else
					echo "<option value=\"0\" >Ciclico</option>";
						
				for($i = 1; $i <= 31; $i++)
				{
					
						
					if ($i != $daysel)
						echo "<option value=\"$i\">$i</option>";
					else
						echo "<option value=\"$i\" selected=\"selected\">$i</option>";
				}
		   ?>
        </select>
       Mes: 
       <select name="mes" id="mes">
         <?
		   		$monthsel = $info["mes"];
				
				if($monthsel == 0)
					echo "<option value=\"0\" selected=\"selected\" >Ciclico</option>";
				else
					echo "<option value=\"0\" >Ciclico</option>";
						
				for($i = 1; $i <= 12; $i++)
				{
					if ($i != $monthsel)
						echo "<option value=\"$i\">$i</option>";
					else
						echo "<option value=\"$i\" selected=\"selected\">$i</option>";
				}
		   ?>
       </select>
        A&ntilde;o: 
        <input name="year" type="text" id="year" size="4" maxlength="4" value="<? echo $info["year"];?>" /> 
        - 
        <br />
        <br />
        Dia fin: 
        <select name="dia_fin" size="1" id="dia_fin">
         <? $daysel = $info["dia_fin"];
				
				if($daysel == 0)
					echo "<option value=\"0\" selected=\"selected\" >Ciclico</option>";
				else
					echo "<option value=\"0\" >Ciclico</option>";
						
				for($i = 1; $i <= 31; $i++)
				{
					
						
					if ($i != $daysel)
						echo "<option value=\"$i\">$i</option>";
					else
						echo "<option value=\"$i\" selected=\"selected\">$i</option>";
				}?>
                </select>
Mes fin:
<select name="mes_fin" id="mes_fin">
  <?
		   		$monthsel = $info["mes_fin"];
				
				if($monthsel == 0)
					echo "<option value=\"0\" selected=\"selected\" >Ciclico</option>";
				else
					echo "<option value=\"0\" >Ciclico</option>";
						
				for($i = 1; $i <= 12; $i++)
				{
					if ($i != $monthsel)
						echo "<option value=\"$i\">$i</option>";
					else
						echo "<option value=\"$i\" selected=\"selected\">$i</option>";
				}
		   ?>
</select>
A&ntilde;o fin:
<input name="year_fin" type="text" id="year_fin" size="4" maxlength="4" value="<? echo $info["year_fin"];?>" />
<br />
       <br /></td>
	 </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>evento_es</strong></td>
      <td class="td_tabladatos_PAR"><textarea name="evento_es" cols="80" rows="10" id="evento_es"><? echo $info["evento_es"];?></textarea>
      <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>evento_en</strong></td>
      <td class="td_tabladatos_PAR"><textarea name="evento_en" cols="80" rows="10" id="evento_en"><? echo $info["evento_en"];?></textarea>
      <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>evento_ca</strong></td>
      <td class="td_tabladatos_PAR"><textarea name="evento_ca" cols="80" rows="10" id="evento_ca"><? echo $info["evento_ca"];?></textarea>
      <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>lugar </strong></td>
      <td class="td_tabladatos_PAR"><br />
      <input name="lugar" type="text" id="lugar" value="<? echo $info["lugar"];?>" /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row">&nbsp;</td>
      <td class="td_tabladatos_PAR"><br />
          <? echo $info["fecha"];?>
          <input type="hidden" name="event" value="<? echo $event;?>"/>
          <input type="hidden" name="action" value="<? echo $action;?>"/>
      </td>
    </tr>
    <tr>
      <td colspan="2" class="td_tabladatos_PAR"><input type="submit" name="Submit" value="guardar" /></td>
    </tr>
  </table>
</form>
</div>
<br />
<br />

	</div>
</div>
<div id="peu">&nbsp;</div>
</div>

</body>
</html>

