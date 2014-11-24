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
	$dic_params = array("(--population_size" => "Population size", "--intermediate_population_size" => "Intermediate population size", "--enviroment_length" => "Environment length", "--enviroment_choice" => "Environment choice");
	
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
				case "--evolution_steps":				
				//case "--enviroment_length":
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
		if($default["min_--max_time_to_spend"] > $params["--max_time_to_spend"])
		{
			$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; The max time to spend should be higher then 1s <p/>";				
		}		
		if($default["min_--cross_extra_parameter"] > $params["--select_cross_extra_parameter"] || $default["max_--cross_extra_parameter"] < $params["--select_cross_extra_parameter"])
		{
			$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; The crossover extra parameter should be between min and max value <p/>";				
		}
		if($default["min_--mutate_extra_parameter"] > $params["--select_mutate_extra_parameter"] || $default["max_--mutate_extra_parameter"] < $params["--select_mutate_extra_parameter"])
		{
			$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; The mutation extra parameter should be between min and max value <p/>";				
		}
		if($default["min_--select_extra_parameter"] > $params["--select_extra_parameter"] || $default["max_--select_extra_parameter"] < $params["--select_extra_parameter"])
		{
			$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; The selection extra parameter should be between min and max value <p/>";				
		}
		if($default["min_--ga_number_metaepochs"] > $params["--ga_number_metaepochs"] || $default["max_--ga_number_metaepochs"] < $params["--ga_number_metaepochs"])
		{
			$errors[] = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; The number of metaepochs should be between min and max value <p/>";	
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
	$params = $cm->getDefaultParamsWithParametro("default_params_HGS");
	
	if(isset($_COOKIE["params"]))
		$old_prams = $_COOKIE["params"];
	else
		header("location:params.php");
	
	checkPermision($cm,$user);
	
	if(isset($_POST["Submit"]))
	{
			$errors = check($_POST,$params);
			if(count($errors) == 0)
			{
				ob_start();
				$strparams = "";
				$new_old_params = "";
				if(isset($_COOKIE["params"]))
				{
					$old_params = $_COOKIE["params"];
				}
				foreach($_POST as $param => $value)
				{
					if($value!="" || $value=="0")
					{
						switch($param)
						{
							case "--number_branches":
								$strparams .= "--hgs_number_branches ".$value." ";
								break;
							case "--ga_number_metaepochs":
								$strparams .= "--hgs_number_metaepochs ".$value." ";
								break;
							case "--evolution_steps":
								$strparams .= "--ga_evolution_steps ".$value." ";
								break;
							case "--population_size":
								$strparams .= "--hgs2_pop_size ".$value." ";
								$strparams .= "--hgs3_pop_size ".$value." ";
								$strparams .= "--hgs4_pop_size ".$value." ";
								$strparams .= "--hgs5_pop_size ".$value." ";
								break;
							case "--intermediate_population_size":
								$strparams .= "--hgs2_intermediate_size ".$value." ";
								$strparams .= "--hgs3_intermediate_size ".$value." ";
								$strparams .= "--hgs4_intermediate_size ".$value." ";
								$strparams .= "--hgs5_intermediate_size ".$value." ";
								break;
							case "--select_choice":
								$strparams .= "--ga_select_choice ".ConvertEnumToInt($value)." ";
								$strparams .= "--hgs2_select_choice ".ConvertEnumToInt($value)." ";
								$strparams .= "--hgs3_select_choice ".ConvertEnumToInt($value)." ";
								$strparams .= "--hgs4_select_choice ".ConvertEnumToInt($value)." ";
								$strparams .= "--hgs5_select_choice ".ConvertEnumToInt($value)." ";
								break;
							case "--mutate_probability":
								$strparams .= "--hgs2_prob_mutate ".$value." ";
								$strparams .= "--hgs3_prob_mutate ".$value." ";
								$strparams .= "--hgs4_prob_mutate ".$value." ";
								$strparams .= "--hgs5_prob_mutate ".$value." ";
								break;
							case "--cross_probability":
								$strparams .= "--hgs2_prob_cross ".$value." ";
								$strparams .= "--hgs3_prob_cross ".$value." ";
								$strparams .= "--hgs4_prob_cross ".$value." ";
								$strparams .= "--hgs5_prob_cross ".$value." ";
								break;
							case "--start_choice":
								$strparams .= "--ga_start_choice ".ConvertEnumToInt($value)." ";
								$strparams .= "--hgs2_start_choice ".ConvertEnumToInt($value)." ";
								$strparams .= "--hgs3_start_choice ".ConvertEnumToInt($value)." ";
								$strparams .= "--hgs4_start_choice ".ConvertEnumToInt($value)." ";
								$strparams .= "--hgs5_start_choice ".ConvertEnumToInt($value)." ";
								break;
							case "--select_extra_parameter":
								$strparams .= "--ga_select_extra ".$value." ";
								$strparams .= "--hgs2_select_extra ".$value." ";	
								$strparams .= "--hgs3_select_extra ".$value." ";	
								$strparams .= "--hgs4_select_extra ".$value." ";	
								$strparams .= "--hgs5_select_extra ".$value." ";	
								break;						
							case "--cross_choice":
								$strparams .= "--ga_cross_choice ".ConvertEnumToInt($value)." ";
								$strparams .= "--hgs2_cross_choice ".ConvertEnumToInt($value)." ";
								$strparams .= "--hgs3_cross_choice ".ConvertEnumToInt($value)." ";
								$strparams .= "--hgs4_cross_choice ".ConvertEnumToInt($value)." ";
								$strparams .= "--hgs5_cross_choice ".ConvertEnumToInt($value)." ";
								break;
							case "--select_cross_extra_parameter":						
								$strparams .= "--ga_cross_extra ".$value." ";
								$strparams .= "--hgs2_cross_extra ".$value." ";	
								$strparams .= "--hgs3_cross_extra ".$value." ";	
								$strparams .= "--hgs4_cross_extra ".$value." ";	
								$strparams .= "--hgs5_cross_extra ".$value." ";	
								break;
							case "--mutate_choice":
								$strparams .= "--ga_mutate_choice ".ConvertEnumToInt($value)." ";
								$strparams .= "--hgs2_mutate_choice ".ConvertEnumToInt($value)." ";
								$strparams .= "--hgs3_mutate_choice ".ConvertEnumToInt($value)." ";
								$strparams .= "--hgs4_mutate_choice ".ConvertEnumToInt($value)." ";
								$strparams .= "--hgs5_mutate_choice ".ConvertEnumToInt($value)." ";
								break;
							case "--select_mutate_extra_parameter":
								$strparams .= "--ga_mutate_extra ".$value." ";
								$strparams .= "--hgs2_mutate_extra ".$value." ";
								$strparams .= "--hgs3_mutate_extra ".$value." ";
								$strparams .= "--hgs4_mutate_extra ".$value." ";
								$strparams .= "--hgs5_mutate_extra ".$value." ";
								break;
							case "--mutate_size":
								$strparams .= "--hgs_mutation_size ".$value." ";
								break;
							case "--replace_if_better":
								if($value == true)
								{
									$strparams .= "--ga_replace_if_better 1 ";
									$strparams .= "--hgs2_replace_if_better 1 ";
									$strparams .= "--hgs3_replace_if_better 1 ";
									$strparams .= "--hgs4_replace_if_better 1 ";
									$strparams .= "--hgs5_replace_if_better 1 ";
								}
								else
								{
									$strparams .= "--ga_replace_if_better 0 ";
									$strparams .= "--hgs2_replace_if_better 0 ";
									$strparams .= "--hgs3_replace_if_better 0 ";
									$strparams .= "--hgs4_replace_if_better 0 ";
									$strparams .= "--hgs5_replace_if_better 0 ";
								}
								break;
							case "--replace_generational":
								if($value == true)
								{
									$strparams .= "--ga_replace_generational 1 ";
									$strparams .= "--hgs2_replace_generational 1 ";
									$strparams .= "--hgs3_replace_generational 1 ";
									$strparams .= "--hgs4_replace_generational 1 ";
									$strparams .= "--hgs5_replace_generational 1 ";
								}
								else
								{
									$strparams .= "--ga_replace_generational 0 ";
									$strparams .= "--hgs2_replace_generational 0 ";
									$strparams .= "--hgs3_replace_generational 0 ";
									$strparams .= "--hgs4_replace_generational 0 ";
									$strparams .= "--hgs5_replace_generational 0 ";
								}
								break;
							case "--start_choice":
								$strparams .= "--hgs2_start_choice ".ConvertEnumToInt($value)." ";
								$strparams .= "--hgs3_start_choice ".ConvertEnumToInt($value)." ";
								$strparams .= "--hgs4_start_choice ".ConvertEnumToInt($value)." ";
								$strparams .= "--hgs5_start_choice ".ConvertEnumToInt($value)." ";
								break;
							case "--enviroment_length":
								$strparams .= "--hgs_env_length ".$value." ";
								break;
							//case "--enviroment_choice":
							//	$strparams .= "--hgs_env_choice ".$value." ";
							//	break;
							//case "--opt_strategy":
							case "--max_time_to_spend":
								$old_params = str_replace("HGS_scheduler", "HGS_scheduler". getGeneticAlgorithmParameters($_POST), $old_params);
								break;
							default:
								break;
						}		
					}
				}
				//echo $strparams;
				setcookie('params', $old_params, time() + 3600);
				setcookie('HGSParams', $strparams, time() + 3600);
				header("location:HGSBranchesParams.php");
				
			/*	$avisos = $cm->getAvisos();
				if($avisos["ejecucion"] == "S") 
				{
					$to = $cm->getMail();
					$message = "El usuario:<strong> ".$_SESSION["login"]["username"]."</strong> ha realizado una ejecucion<br><br> Administracion WEBGRID"; 
					$alt_message = "El usuario:".$_SESSION["login"]["username"]."</strong> ha realizado una nueva ejecucion\n\n Administracion WEBGRID"; 
					$mail = new Mail("WEB GRID@UPC",$to,"Aviso: nueva ejecucion",$message,$alt_message, "../../lib/phpmailer/");
					$mail->send();
				}*/
				//header("location:message_params.php?message=ok");	
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
<h1>Hierarchic Genetic Strategy general settings</h1>
<div id="error" align="center">
	<?
		for($i = 0; $i < count($errors); $i++)
		{
			echo $errors[$i];
		}	
	?>
</div>
<form id="form1" name="form1" method="post" action="HGSParams.php">
  <table width="80%" cellpadding="0" cellspacing="0" class="tabladatos">
  <tr>
  	<th class="th_tabladatos" scope="row">Maximal degree of branch</th>
  	<td class="td_tabladatos_PAR" colspan="4">
  		<select name="--number_branches" id="--number_branches">
  		<?
  			if(isset($_POST["--number_branches"]))
  				getOptions(array("2","3","4","5"), array("2","3","4","5"), $_POST["--number_branches"]);
  			else
  				getOptions(array("2","3","4","5"), array("2","3","4","5"), $params["--number_branches"]);
  		?>
  		</select>
  	</td>
  </tr>
  	<tr>
  		<th class="th_tabladatos" scope="row">Number of metaepochs</th>
  		<td class="td_tabladatos_PAR" colspan="3">
  			<input name="--ga_number_metaepochs" id="--ga_number_metaepochs" type="text" size="4" value="<?if(isset($_POST["--ga_number_metaepochs"]))
  				echo $_POST["--ga_number_metaepochs"];
  			 else 
  			 	echo $params["--ga_number_metaepochs"];?>"/>
  		</td>
        <td class="td_tabladatos_PAR" colspan="3">
  		(integer)(min: <? echo $params["min_--ga_number_metaepochs"]; ?>, max: <? echo $params["max_--ga_number_metaepochs"]; ?>)
  	</td>
  	</tr>
  	<tr>
  	<th class="th_tabladatos" scope="row">Metaepoch length</th>
  	<td class="td_tabladatos_PAR" colspan="3">
  		<input name="--evolution_steps" id="--evolution_steps" type="text" size="4" value="<?if(isset($_POST["--evolution_steps"]))
  				echo $_POST["--evolution_steps"];
  			 else 
  			 	echo $params["--evolution_steps"];?>"/>
  	</td>
  	<td class="td_tabladatos_PAR" colspan="3">
  		(integer)(min: <? echo $params["min_--evolution_steps"]; ?>, max: <? echo $params["max_--evolution_steps"]; ?>)
  	</td>
  	</tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Max time to spend</strong></td>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--max_time_to_spend" type="text" id="--max_time_to_spend" size="4" value="<?if(isset($_POST["--max_time_to_spend"]))
  				echo $_POST["--max_time_to_spend"];
  			 else 
  			 	echo $params["--max_time_to_spend"];?>" /></td>
	  <td class="td_tabladatos_PAR" colspan="2">
        (integer) (min: 1s)</td>
    </tr>
    <tr>
      	<th class="th_tabladatos" scope="row">
      		<strong>Optimization strategy</strong>
     	</th>
      	<td class="td_tabladatos_PAR" colspan="2">
      		<select name="--opt_strategy" id="--opt_strategy" onchange="loadProperties('params_workload',this)";>
	  		
			<? 
				if(isset($_POST["--opt_strategy"]))
					getOptions(array("true", "false"),array("1", "0"),$_POST["--opt_strategy"]);
				
				else
					getOptions(array("true", "false"),array("1", "0"),$params["--opt_strategy"]);
			?>
						</select>
		</td>
		<td class="td_tabladatos_PAR" colspan="2">true - hierarchic<br/> false - simultaneous</td>
    </tr> 
   <tr>
      <th class="th_tabladatos" scope="row"><strong>Selection choice</strong></th>
      <td class="td_tabladatos_PAR" colspan="2"><select name="--select_choice" id="--select_choice" >
	  	<? 
			if(isset($_POST["--select_choice"]))
				getOptions(array("SelectRandom", "SelectBest", "SelectLinearRanking", "SelectExpRanking", "SelectBinaryTournament",
                         "SelectNTournament"),array("SelectRandom", "SelectBest", "SelectLinearRanking", "SelectExpRanking", "SelectBinaryTournament",
                         "SelectNTournament"),$_POST["--select_choice"]);
			else
				getOptions(array("SelectRandom", "SelectBest", "SelectLinearRanking", "SelectExpRanking", "SelectBinaryTournament",
                         "SelectNTournament"),array("SelectRandom", "SelectBest", "SelectLinearRanking", "SelectExpRanking", "SelectBinaryTournament",
                         "SelectNTournament"),$params["--select_choice"]);	
		?>
	  </select>
        </td>
        <td class="td_tabladatos_PAR" colspan="2">
        </td>
    </tr>
   <tr>
      <th class="th_tabladatos" scope="row"><strong>Selection extra parameter</strong></th>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--select_extra_parameter" type="text" id="--select_extra_parameter" size="4" value="<?
	  	if(isset($_POST["--select_extra_parameter"]))
	  		echo $_POST["--select_extra_parameter"];
		else
			echo $params["--select_extra_parameter"];?>"/> </td>
			<td class="td_tabladatos_PAR" colspan="2">
        (double)(min: <?echo $params["min_--select_extra_parameter"]; ?>, max: <?echo $params["max_--select_extra_parameter"]; ?>)</td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Selection crossover choice</strong></th>
      <td class="td_tabladatos_PAR" colspan="2"><select name="--cross_choice" id="--cross_choice" >
	  	<? 
			if(isset($_POST["--cross_choice"]))
				getOptions(array("CrossOnePoint", "CrossTwoPoints", "CrossUniform",
    					 "CrossFitnessBased", "CrossPMX", "CrossOX", "CrossCX"), array("CrossOnePoint", "CrossTwoPoints", "CrossUniform",
    					 "CrossFitnessBased", "CrossPMX", "CrossOX", "CrossCX"),$_POST["--cross_choice"]);
			else
				getOptions(array("CrossOnePoint", "CrossTwoPoints", "CrossUniform",
    					 "CrossFitnessBased", "CrossPMX", "CrossOX", "CrossCX"),array("CrossOnePoint", "CrossTwoPoints", "CrossUniform",
    					 "CrossFitnessBased", "CrossPMX", "CrossOX", "CrossCX"),$params["--cross_choice"]);	
		?>
	  </select>
        </td>
        <td class="td_tabladatos_PAR" colspan="2">
        </td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Crossover extra parameter</strong></th>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--select_cross_extra_parameter" type="text" id="--select_cross_extra_parameter" size="4" value="<?
	  	if(isset($_POST["--cross_extra_parameter"]))
	  		echo $_POST["--cross_extra_parameter"];
		else
			echo $params["--cross_extra_parameter"];?>"/> </td>
			<td class="td_tabladatos_PAR" colspan="2">
        (double)(min: <?echo $params["min_--cross_extra_parameter"]; ?>, max: <?echo $params["max_--cross_extra_parameter"]; ?>)</td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Selection mutation operator</strong></th>
      <td class="td_tabladatos_PAR" colspan="2"><select name="--mutate_choice" id="--mutate_choice" >
	  	<? 
			if(isset($_POST["--mutate_choice"]))
				getOptions(array("MutateMove", "MutateSwap", "MutateBoth", 
    					 "MutateRebalancing"), array("MutateMove", "MutateSwap", "MutateBoth", 
    					 "MutateRebalancing"),$_POST["--mutate_choice"]);
			else
				getOptions(array("MutateMove", "MutateSwap", "MutateBoth", 
    					 "MutateRebalancing"),array("MutateMove", "MutateSwap", "MutateBoth", 
    					 "MutateRebalancing"),$params["--mutate_choice"]);	
		?>
	  </select>
        </td>
        <td class="td_tabladatos_PAR" colspan="2">
        </td>
    </tr>
     <tr>
      <th class="th_tabladatos" scope="row"><strong>Mutation extra parameter</strong></th>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--select_mutate_extra_parameter" type="text" id="--select_mutate_extra_parameter" size="4" value="<?
	  	if(isset($_POST["--mutate_extra_parameter"]))
	  		echo $_POST["--mutate_extra_parameter"];
		else
			echo $params["--mutate_extra_parameter"];?>"/> </td>
			<td class="td_tabladatos_PAR" colspan="2">
        (double)(min: <? echo $params["min_--mutate_extra_parameter"]; ?>, max: <? echo $params["max_--mutate_extra_parameter"]; ?>)</td>
    </tr>
    <tr>
  		<th class="th_tabladatos" scope="row">Enviroment length</th>
  		<td class="td_tabladatos_PAR" colspan="3">
  			<input name="--enviroment_length" id="--enviroment_length" type="text" size="4" value="<?if(isset($_POST["--enviroment_length"]))
  				echo $_POST["--enviroment_length"];
  			 else 
  			 	echo $params["--enviroment_length"];?>"/>
  		</td>
    	<td class="td_tabladatos_PAR">(double)(min: <?echo $params["min_--enviroment_length"]; ?>, max: <?echo $params["max_--enviroment_length"]; ?>)</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row" colspan="5">Replacement methods parameters</th>
    </tr>
    <tr>
      	<th class="th_tabladatos" scope="row">
      		<strong>Replace only if better</strong>
     	</th>
      	<td class="td_tabladatos_PAR" colspan="2">
      		<select name="--replace_if_better" id="--replace_if_better" onchange="loadProperties('params_workload',this)";>
	  		
			<? 
				if(isset($_POST["--replace_if_better"]))
					getOptions(array("false", "true"),array("0", "1"),$_POST["--replace_if_better"]);
				
				else
					getOptions(array("false", "true"),array("0", "1"),$params["--replace_if_better"]);
			?>
						</select>
		</td>
		<td class="td_tabladatos_PAR" colspan="2">
			true - Replace if Better <br/> false - Steady State, Replace if Better, Elitist Generational
													</td>
    <tr>
      	<th class="th_tabladatos" scope="row">
      		<strong>Replace generational</strong>
     	</th>
      	<td class="td_tabladatos_PAR" colspan="2">
      		<select name="--replace_generational" id="--replace_generational" onchange="loadProperties('params_workload',this)";>
	  		
			<? 
				if(isset($_POST["--replace_generational"]))
					getOptions(array("false", "true"),array("0", "1"),$_POST["--replace_generational"]);
				
				else
					getOptions(array("false", "true"),array("0", "1"),$params["--replace_generational"]);
			?>
						</select>
		</td>
		<td class="td_tabladatos_PAR" colspan="2">
			true - Simple Generational <br/> false - Steady State, Replace if Better, Elitist Generational
		</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row" colspan="5">Initialization method</th>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Selection start choice</strong></th>
      <td class="td_tabladatos_PAR" colspan="2"><select name="--start_choice" id="--start_choice" >
	  	<? 
			if(isset($_POST["--start_choice"]))
				getOptions(array("StartLJFRSJFR", "StartMCT", "StartRandom", "StartAll",
                        "StartMinMin", "StartRelativeCost", "StartKPB", "StartAll2"), array("StartLJFRSJFR", "StartMCT", "StartRandom", "StartAll",
                        "StartMinMin", "StartRelativeCost", "StartKPB", "StartAll2"),$_POST["--start_choice"]);
			else
				getOptions(array("StartLJFRSJFR", "StartMCT", "StartRandom", "StartAll",
                        "StartMinMin", "StartRelativeCost", "StartKPB", "StartAll2"), array("StartLJFRSJFR", "StartMCT", "StartRandom", "StartAll",
                        "StartMinMin", "StartRelativeCost", "StartKPB", "StartAll2"),$params["--start_choice"]);	
		?>
	  </select>
        </td>
        <td class="td_tabladatos_PAR" colspan="2">
        </td>
    </tr>
  </table>
  <br />
  <input type="submit" name="Submit" value="Next" />
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
