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
<?
	$dic_params = array("(--population_size" => "Population size", "--intermediate_population_size" => "Intermediate population size", "--enviroment_length" => "Enviroment length", "--enviroment_choice" => "Enviroment choice");
	
	include "../../management/configManager.php";
	include "../../management/userManager.php";
	include "../../admin/config.php";
	include "genExecution.php";
	require "../../lib/phpmailer/class.phpmailer.php";
	include "../../lib/Mail.php";
	
	function checkPermision()
	{
		$user = $_SESSION["login"]["username"];

		$userm = new userManager();
		$cm = new configManager();
		
		$max = $cm->getMaxExecutions();
		
		$actual = getdate(time());
		$ini = mktime(0,0,0,$actual["mon"],$actual["mday"],$actual["year"]);
		$fin = mktime(23,59,59,$actual["mon"],$actual["mday"],$actual["year"]);
		
		$executions = $userm->getNumberExecutions($ini,$fin,$user); 
		//echo "max:$max - executions:$executions";
		if($max < $executions)
			header("location:message_params.php?message=max");
		
		//echo $cm->getMaxExecutions()." - ". $userm->getNumberExecutions($ini,$fin,$user);
	}
	
	
	function checkIntegers(&$params, $default,&$errors)
	{
		global $dic_params;
	
		foreach($params as $key => $value)
		{
			switch($key)
			{						
				case "--population_size":			
				case "--intermediate_population_size":					
								if(!is_numeric($value))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; $dic_params[$key]  	value should be numeric <p/>";				
								else
								{
									$params[$key] = intval($value);
									if($default["max_".$key] < $value || $default["min_".$key] > $value)
										$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; $dic_params[$key]  	must be in the interval: [".$default["min_".$key]." - ".$default["max_".$key]."]<p/>";
								}								
								break;
			}
		}	
		if($params["--hgs_prob_cross"] > $default["max_--hgs_prob_cross"] || $params["--hgs_prob_cross"] < $default["min_--hgs_prob_cross"])
		{
			$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; The crossover probability should be between min and max value<p/>";				
		}
		if($params["--hgs_prob_mutate"] > $default["max_--hgs_prob_mutate"] || $params["--hgs_prob_mutate"] < $default["min_--hgs_prob_mutate"])
		{
			$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; The mutation probability should be between min and max value<p/>";				
		}	
	}
	
	function checkDistributionValues(&$params,&$errors)
	{
		
		global $dic_params;
		foreach($params as $key => $value)
		{
			
			switch($value)
			{	
				case "c":
							
							if(!is_numeric($params[$key."_constant"]) && $params[$key] != "0")
								$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; The  $dic_params[$key] constant value should be numeric<p/>";			 
							
							break;
				case "u":
							if(!is_numeric($params[$key."_low"]))
								$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;  The  $dic_params[$key] low value should be numeric <p/>";
							
							if(!is_numeric($params[$key."_high"]))
								$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;  The  $dic_params[$key] high value should be numeric <p/>";
											 
							break;
				case "e":
								if(!is_numeric($params[$key."_mean"]))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;  The  $dic_params[$key] mean value should be numeric <p/>";
							break;
				case "zipf":
							if(!is_numeric($params[$key."_x1"]))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;  The  $dic_params[$key] x1 value should be numeric <p/>";
							
							if(!is_numeric($params[$key."_x2"]))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;  The  $dic_params[$key] x2 value should be numeric <p/>";
									
							if(!is_numeric($params[$key."_x3"]))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;  The  $dic_params[$key] x3 value should be numeric <p/>";				
							break;
				case "n":
							if(!is_numeric($params[$key."_mean"]))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;  The  $dic_params[$key] mean value should be numeric <p/>";				
							$str .= "n(".$params[$param."_mean"].",".$params[$param."_sd"].")";
							break;
							
							if(!is_numeric($params[$key."_sd"]))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;  The  $dic_params[$key] sd value should be numeric <p/>";				
				
				case "triangle":
							if(!is_numeric($params[$key."_a"]))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;  The  $dic_params[$key] a value should be numeric <p/>";
							
							if(!is_numeric($params[$key."_b"]))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;  The  $dic_params[$key] b value should be numerico <p/>";
									
							if(!is_numeric($params[$key."_c"]))
									$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; The  $dic_params[$key] c value should be numeric <p/>";		
							$params[$key."_a"] = intval($params[$key."_a"]);						
							$params[$key."_b"] = intval($params[$key."_b"]);
							$params[$key."_c"] = intval($params[$key."_c"]);														
							break;
			}
		}	
	}
	
	function check(&$params,$default)
	{
		
		$errors = array();
		checkIntegers($params,$default,$errors);
		//checkDistributionValues($params,$errors);
		return $errors;
	}
	
	$cm = new configManager();
	$params = $cm->getDefaultParamsWithParametro("default_params_HGSBranches");
	
	if(isset($_COOKIE["params"]) && isset($_COOKIE["HGSParams"]))
	{	
		$old_params = $_COOKIE["params"];
		$HGSParams = $_COOKIE["HGSParams"];
		//echo $HGSParams;
	}
	else
	{
		header("location:params.php");
	}
	
	checkPermision($cm,$user);
	
	if(isset($_POST["Submit"]))
	{
			$errors = check($_POST,$params);
			if(count($errors) == 0)
			{
				if(isset($_COOKIE["params"]) && isset($_COOKIE["HGSParams"]))
				{	
					$old_params = $_COOKIE["params"];
					$HGSParams = $_COOKIE["HGSParams"];
				}
				ob_start();
				//echo $old_params."<br/>";
				generateExecution($_SESSION["login"]["username"],$_POST, $old_params." ".$HGSParams, 1);
			/*	$avisos = $cm->getAvisos();
				if($avisos["ejecucion"] == "S") 
				{
					$to = $cm->getMail();
					$message = "El usuario:<strong> ".$_SESSION["login"]["username"]."</strong> ha realizado una ejecucion<br><br> Administracion WEBGRID"; 
					$alt_message = "El usuario:".$_SESSION["login"]["username"]."</strong> ha realizado una nueva ejecucion\n\n Administracion WEBGRID"; 
					$mail = new Mail("WEB GRID@UPC",$to,"Aviso: nueva ejecucion",$message,$alt_message, "../../lib/phpmailer/");
					$mail->send();
				}*/
				header("location:message_params.php?message=ok");	
				ob_end_flush();
			}	
			
				
			
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
<script language="JavaScript" type="text/javascript" >
function loadProperties(name,selected)
{
	div = document.getElementById(name);
	var html;
	var name1,name2,name3;
	switch(selected.value)
	{
		case "c":
					name1 = selected.name +  '_constant';
					html = 'constant: <input name=\"'+ name1 +'\" type=\"text\" size=\"4\" />';
					break;
		case "u":
					name1 = selected.name +  '_low';
					name2 = selected.name +  '_high';
					html = 'low: <input name=\"'+ name1 + '\" type=\"text\" size=\"4\" /> high: <input name=\"' + name2 + '\" type=\"text\" size=\"4\" />';
					break;
		case "n":
					name1 = selected.name +  '_mean';
					name2 = selected.name +  '_sd';
					html = 'mean: <input name=\"' + name1 +'\" type=\"text\" size=\"4\" /> sd: <input name=\"' + name2 + '\" type=\"text\" size=\"4\" />';
					break;
		case "e":
					name1 = selected.name + '_mean';
					html = 'mean: <input name=\"'+name1+'\" type=\"text\" size=\"4\" />';
					break;
					
		case "triangle":
						name1 = selected.name +  '_a';
						name2 = selected.name +  '_b';
						name3 = selected.name +  '_c';
						html = 'a: <input name=\"' + name1 + '\" type=\"text\" size=\"4\" /> b: <input name=\"'+ name2 +'\" type=\"text\" size=\"4\" /> c: <input name=\"' + name3 +'\" type=\"text\" size=\"4\" />';
							break;
		case "zipf":	
						name1 = selected.name +  '_x1';
						name2 = selected.name +  '_x2';
						name3 = selected.name +  '_p';
						html = 'x1: <input name=\"'+ name1 +'\" type=\"text\" size=\"4\" /> x2: <input name=\"'+ name2 +'\" type=\"text\" size=\"4\" /> p: <input name=\"'+ name3 +'\" type=\"text\" size=\"4\" />';
						break;
	}	
	//alert(html);
	div.innerHTML = html;		
}	
</script>

</head>

<body>
<?
	
	function getOptions($keys, $values, $selected)
	{
			
		for($i = 0; $i < count($keys); $i++)
		{
				if($values!= "")
				{
					if ($values[$i] != $selected)
						echo "<option value=\"$values[$i]\">$keys[$i]</option>";
					else
						echo "<option value=\"$values[$i]\" selected=\"selected\">$keys[$i]</option>";
				}
				
				
		}
	}
	function getProperties($key, $value, $params)
	{
		switch($value)
			{
				case "c": 
							echo "constant: <input name=\"".$key."_constant\" type=\"text\" size=\"4\" value=\"".$params[$key."_constant"]."\"/>";
							break;
				case "u":	
							echo "low: <input name=\"".$key."_low\" type=\"text\" size=\"4\"  value\"".$params[$key."_low"]."\"/> 
							high: <input name=\"".$key."_high\" type=\"text\" size=\"4\" value\"".$params[$key."_high"]."\"/>";
							break;
								
							
				case "e":
							echo "mean: <input name=\"".$key."_mean\" type=\"text\" size=\"4\" value=\"".$params[$key."_mean"]."\"/>";
							break;	
				case "zipf":
								echo "x1: <input name=\"".$key."_x1\" type=\"text\" size=\"4\" value=\"".$params[$key."_x1"]."\"/> 
									x2: <input name=\"".$key."_x2\" type=\"text\" size=\"4\" value=\"".$params[$key."_x2"]."\"/> 
									p: <input name=\"".$key."_p\" type=\"text\" size=\"4\" value=\"".$params[$key."_p"]."\"/>";
								break;
								
				case "triangle":
									echo "a: <input name=\"".$key."_a\" type=\"text\" size=\"4\" value=\"".$params[$key."_a"]."\"/> 
									b: <input name=\"".$key."_b\" type=\"text\" size=\"4\" value=\"".$params[$key."_b"]."\"/> 
									c: <input name=\"".$key."_c\" type=\"text\" size=\"4\" value=\"".$params[$key."_c"]."\"/>";
									break;
			}
	}
	
	function CreateForms($number_branches)
	{
		for($i = 2; $i < $number_branches; $i++)
		{
			echo "<h2> Branch setting: Branch($i)</h2>
			<table width=\"80%\" cellpadding=\"0\" cellspacing=\"0\" class=\"tabladatos\">
  				<tr>
  					<th class=\"th_tabladatos\" scope=\"row\">Population size</th>
  					<td class=\"td_tabladatos_PAR\" colspan=\"2\">
  						<input name=\"--hgs".($i) ."_pop_size\" id=\"--hgs".($i) ."_pop_size\" type=\"text\" size=\"4\" value=\"18\"/>
  				</td>
  				<td class=\"td_tabladatos_PAR\">(integer)(smaller than in ROOT)</td>
  		</tr>
    <tr>
    	<th class=\"th_tabladatos\" scope=\"row\"><strong>Intermediate population size</strong></th>
    	<td class=\"td_tabladatos_PAR\" colspan=\"2\">
    	<input name=\"--hgs".($i)."_intermediate_size\" id=\"--hgs".($i)."_intermediate_size\" type=\"text\" size=\"4\" value=\"16\" />
    	</td>
    	<td class=\"td_tabladatos_PAR\">(integer)(smaller than in ROOT)</td>
    </tr>
    <tr>
      <th class=\"th_tabladatos\" scope=\"row\"><strong>Crossover probability </strong></td>
      <td class=\"td_tabladatos_PAR\" colspan=\"2\">
      	<input name=\"--hgs".($i)."_prob_cross\" id=\"--cross_probability\" type=\"text\" size=\"4\" value=\"0.80\"/>
		</td>
		<td class=\"td_tabladatos_PAR\">
			(double) (min: 0, max: 1)
		</td>
    </tr>
    <tr>
      <th class=\"th_tabladatos\" scope=\"row\"><strong>Mutation probability</strong></th>
      <td class=\"td_tabladatos_PAR\" colspan=\"2\">
      	<input name=\"--hgs".($i)."_prob_mutate\" id=\"--hgs".($i)."_prob_mutate\" type=\"text\" size=\"4\" value=\"0.20\"/>
		  </td>
		<td class=\"td_tabladatos_PAR\" >
		(double) (min: 0, max: 1)(smaller than in ROOT)</td>
   </tr>
  </table>";
		}
	}
	
	function getValue($name)
	{
		if(isset($_POST[$name]))
		{
			return $_POST[$name];
		}
		else
		{
			return $params[$name];
		}
	}
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
			<li class="pestanas"><strong><a href="preferences.php">Settings</a></strong></li>
		</ul>
	</div>
	<div id="parte_central">
	<div id="menu_interior">
		<ul class="ul_segundomenu">
			<li class="li_segundomenu"><a href="executions.php"> My executions</a></li>
			<li class="li_segundomenu"><span class="segundomenu_selec">New execution</span></li>
		</ul>
	</div>	 
<div id="result" align="center">
<h1>Hierarchic Genetic Strategy settings of branches</h1>
<div id="error" align="center">
	<?
		for($i = 0; $i < count($errors); $i++)
		{
			echo $errors[$i];
		}	
	?>
</div>
<form id="form1" name="form1" method="post" action="HGSBranchesParams.php">
<h2>Branch setting: Root</h2>
  <table width="80%" cellpadding="0" cellspacing="0" class="tabladatos">
  	<tr>
  		<th class="th_tabladatos" scope="row">Population size</th>
  		<td class="td_tabladatos_PAR" colspan="2">
  			<input name="--hgs_pop_size" id="--hgs_pop_size" type="text" size="4" value="<? if(isset($_POST["--hgs_pop_size"]))
  				echo $_POST["--hgs_pop_size"];
  				else
  				echo $params["--hgs_pop_size"]; ?>" />
  		</td>
  		<td class="td_tabladatos_PAR">(integer)</td>
  	</tr>
    <tr>
    <th class="th_tabladatos" scope="row"><strong>Intermediate population size</strong></th>
    <td class="td_tabladatos_PAR" colspan="3">
    	<input name="--hgs_intermediate_size" id="--hgs_intermediate_size" type="text" size="4" value="<? if(isset($_POST["--hgs_intermediate_size"]))
  				echo $_POST["--hgs_intermediate_size"];
  				else
  				echo $params["--hgs_intermediate_size"]; ?>" />
    </td>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Cross probability </strong></td>
      <td class="td_tabladatos_PAR" colspan="2">
      	<input name="--hgs_prob_cross" id="--hgs_prob_cross" type="text" size="4" value="<? if(isset($_POST["--hgs_prob_cross"]))
  				echo $_POST["--hgs_prob_cross"];
  				else
  				echo $params["--hgs_prob_cross"]; ?>" />
		</td>
		<td class="td_tabladatos_PAR">
			(double) (min: <? echo $params["min_--hgs_prob_cross"]; ?>, max: <? echo $params["max_--hgs_prob_cross"]; ?>)
		</td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Mutate probability</strong></th>
      <td class="td_tabladatos_PAR" colspan="2">
      	<input name="--hgs_prob_mutate" id="--hgs_prob_mutate" type="text" size="4" value="<? if(isset($_POST["--hgs_prob_mutate"]))
  				echo $_POST["--hgs_prob_mutate"];
  				else
  				echo $params["--hgs_prob_mutate"]; ?>" />
		  </td>
		<td class="td_tabladatos_PAR" >
		(double) (min: <? echo $params["min_--hgs_prob_mutate"]; ?>, max: <? echo $params["max_--hgs_prob_mutate"]; ?>)</td>
   </tr>
  </table>
  <br />
  <?
  	CreateForms($HGSParams[22] + 1);
   ?>
  <input type="submit" name="Submit" value="Run" />
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
