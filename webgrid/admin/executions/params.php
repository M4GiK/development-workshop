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
<script language="JavaScript" type="text/javascript">
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
				case "n":
							echo "mean: <input name=\"".$key."_mean\" type=\"text\" size=\"4\" value=\"".$params[$key."_mean"]."\"/> 
									sd: <input name=\"".$key."_sd\"\" type=\"text\" size=\"4\" value=\"".$params[$key."_sd"]."\" />";
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
	
	include "../../management/configManager.php";
	include "../../management/incidenceManager.php";
	include "../config.php";
	include "../menus.php";
	
	$cm = new configManager();
	$params = $cm->getDefaultParams();
	$params += $cm->getDefaultParamsWithParametro("default_params_GA");
	$params += $cm->getDefaultParamsWithParametro("default_params_HGS");
	$params += $cm->getDefaultParamsWithParametro("default_params_HGSBranches");
	$params += $cm->getDefaultParamsWithParametro("default_params_TS");
	$params += $cm->getDefaultParamsWithParametro("default_params_SG");
	
	/*foreach($params as $param => $value)
	{
		echo $param." ".$value."<br/>";
	}*/
	
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
			<li class="li_segundomenu"><a href="index.php">Ver ejecuciones</a></li>
			<li class="li_segundomenu"><a href="stats.php">Ver estad&iacute;sticas</a></li>
			<li class="li_segundomenu"><span class="segundomenu_selec">Parametros</span></li>
		</ul>
	</div>	 
<div id="result" align="center">
<h1>Parametros simulador</h1>
<form id="form1" name="form1" method="post" action="saveParams.php">
  <table width="90%" cellpadding="0" cellspacing="0" class="tabladatos">
    
	<tr>
	<th class="td_tabladatos_PAR">&nbsp;</th>
	<th class="th_tabladatos" scope="row" colspan="2">Default</th>
	<th class="th_tabladatos" scope="row" colspan="2">Max</th>
	<th class="th_tabladatos" scope="row" colspan="2">Min</th>
	</tr>
	<tr>
      <th class="th_tabladatos" scope="row"><strong>Numero de tareas </strong></td>
      <td class="td_tabladatos_PAR" colspan="2" ><input name="--ttasks" type="text" id="--ttasks" size="4" value="<? echo $params["--ttasks"];?>"/>
        (integer)</td>
		<td class="td_tabladatos_PAR" colspan="2" ><input name="max_--ttasks" type="text" id="--ttasks" size="4" value="<? echo $params["max_--ttasks"];?>"/></td>
		<td class="td_tabladatos_PAR" colspan="2" ><input name="min_--ttasks" type="text" id="--ttasks" size="4" value="<? echo $params["min_--ttasks"];?>"/></td>
		
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Numero inicial de tareas </strong></td>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--itasks" type="text" id="--itasks" size="4" value="<? echo $params["--itasks"];?>"/>
        (integer)</td>
		<td class="td_tabladatos_PAR" colspan="2" ><input name="max_--itasks" type="text" id="--itasks" size="4" value="<? echo $params["max_--itasks"];?>"/></td>
		<td class="td_tabladatos_PAR" colspan="2" ><input name="min_--itasks" type="text" id="--itasks" size="4" value="<? echo $params["min_--itasks"];?>"/></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Distribucion llegadas al sistema </strong></td>
      <td class="td_tabladatos_PAR"><select name="--iatime" id="--iatime" onchange="loadProperties('params_iatime',this)"; >
          <? getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$params["--iatime"]);?>
        </select>
      </td>
      <td class="td_tabladatos_PAR"><div id="params_iatime">
          <?
			getProperties("--iatime",$params["--iatime"],$params);
		?>
      </div></td>
	  <td class="td_tabladatos_PAR" colspan="2" ></td>
		<td class="td_tabladatos_PAR" colspan="2" ></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Distribucion carga trabajo </strong></td>
      <td class="td_tabladatos_PAR" ><select name="--workload" id="--workload" onchange="loadProperties('params_workload',this)";>
          <? getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$params["--workload"]);?>
        </select>
		
      </td>
      <td class="td_tabladatos_PAR"><div id="params_workload" >
          <?
			getProperties("--workload",$params["--workload"],$params);
		?>
      </div></td>
	  <td class="td_tabladatos_PAR" colspan="2" ></td>
		<td class="td_tabladatos_PAR" colspan="2" ></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Numero inicial maquinas entorno </strong></td>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--ihosts" type="text" id="--ihosts" size="4" value="<? echo $params["--ihosts"];?>" />
        (integer)</td>
		<td class="td_tabladatos_PAR" colspan="2" ><input name="max_--ihosts" type="text" id="--ihosts" size="4" value="<? echo $params["max_--ihosts"];?>" /></td>
		<td class="td_tabladatos_PAR" colspan="2" ><input name="min_--ihosts" type="text" id="--ihosts" size="4" value="<? echo $params["min_--ihosts"];?>" /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Distribucion mips </strong></td>
      <td class="td_tabladatos_PAR" ><select name="--mips" id="--mips" onchange="loadProperties('params_mips',this)">
          <? getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$params["--mips"]);?>
        </select>
      </td>
      <td class="td_tabladatos_PAR"><div id="params_mips" >
          <?
			getProperties("--mips",$params["--mips"],$params);
		?>
      </div></td>
	  <td class="td_tabladatos_PAR" colspan="2" ></td>
		<td class="td_tabladatos_PAR" colspan="2" ></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Distribucion intervalo tiempo alta recursos </strong></td>
      <td class="td_tabladatos_PAR"><select name="--addhost" id="--addhost" onchange="loadProperties('params_addhost',this)">
          <? getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$params["--addhost"]);?>
        </select>
      </td>
      <td class="td_tabladatos_PAR"><div id="params_addhost" >
          <?
			getProperties("--addhost",$params["--addhost"],$params);
		?>
      </div></td>
	  <td class="td_tabladatos_PAR" colspan="2" ></td>
		<td class="td_tabladatos_PAR" colspan="2" ></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Distribucion intervalo de tiempo baja recursos </strong></td>
      <td class="td_tabladatos_PAR"><select name="--delhost" id="--delhost" onchange="loadProperties('params_delhost',this)">
        <option value="c">constant</option>
          <? getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$params["--delhost"]);?>
      </select></td>
      <td class="td_tabladatos_PAR"><div id="params_delhost" >
          <?
			getProperties("--delhost",$params["--delhost"],$params);
		?>
      </div></td>
	  <td class="td_tabladatos_PAR" colspan="2" ></td>
		<td class="td_tabladatos_PAR" colspan="2" ></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Minimo numero de maquinas sistema </strong></td>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--minhosts" type="text" id="--minhosts" size="4" value="<? echo $params["--minhosts"];?>" />
        (integer)</td>
		<td class="td_tabladatos_PAR" colspan="2" ><input name="max_--minhosts" type="text" id="max_--minhosts" size="4" value="<? echo $params["max_--minhosts"];?>" /></td>
		<td class="td_tabladatos_PAR" colspan="2" ><input name="min_--minhosts" type="text" id="min_--minhosts" size="4" value="<? echo $params["min_--minhosts"];?>" /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Maximo numero de maquinas del sistema </strong></td>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--maxhosts" type="text" id="--maxhosts" size="4" value="<? echo $params["--maxhosts"];?>"/>
        (integer)</td>
		<td class="td_tabladatos_PAR" colspan="2" ><input name="max_--maxhosts" type="text" id="--maxhosts" size="4" value="<? echo $params["max_--maxhosts"];?>"/></td>
		<td class="td_tabladatos_PAR" colspan="2" ><input name="min_--maxhosts" type="text" id="--maxhosts" size="4" value="<? echo $params["min_--maxhosts"];?>"/></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Activar replanificacion </strong></td>
      <td class="td_tabladatos_PAR" colspan="2"><select name="--reschedule" id="--reschedule" >
          <? getOptions(array("yes","no"),array("Y","N"),$params["--reschedule"]);?>
      </select></td>
	  <td class="td_tabladatos_PAR" colspan="2" ></td>
		<td class="td_tabladatos_PAR" colspan="2" ></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Politica seleccion recursos </strong></td>
      <td class="td_tabladatos_PAR" colspan="2"><select name="--hostselect" id="--hostselect" >
          <? getOptions(array("idle","all"),array("idle","all"),$params["--hostselect"]);?>
      </select></td>
	  <td class="td_tabladatos_PAR" colspan="2" ></td>
		<td class="td_tabladatos_PAR" colspan="2" ></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Politica activacion planificador </strong></td>
      <td class="td_tabladatos_PAR" ><select name="--activate" id="--activate" onchange="loadProperties('params_activate',this)" >
          <? getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$params["--activate"]);?>
      </select></td>
      <td class="td_tabladatos_PAR"><div id="params_activate" >
          <?
			getProperties("--activate",$params["--activate"],$params);
		?>
      </div></td>
	  <td class="td_tabladatos_PAR" colspan="2" ></td>
		<td class="td_tabladatos_PAR" colspan="2" ></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Politica local gestion tareas de cada host </strong></td>
      <td class="td_tabladatos_PAR" colspan="2"><select name="--allocpolicy" id="--allocpolicy" >
          <? getOptions(array("fcfs","estf"),array("fcfs","estf"),$params["--allocpolicy"]);?>
      </select></td>
	  <td class="td_tabladatos_PAR" colspan="2" ></td>
		<td class="td_tabladatos_PAR" colspan="2" ></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Ejecuciones independientes </strong></td>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--nruns" type="text" id="--nruns" size="4" value="<? echo $params["--nruns"];?>"/>
        (integer)</td>
		<td class="td_tabladatos_PAR" colspan="2" ><input name="max_--nruns" type="text" id="--nruns" size="4" value="<? echo $params["max_--nruns"];?>"/></td>
		<td class="td_tabladatos_PAR" colspan="2" ><input name="min_--nruns" type="text" id="--nruns" size="4" value="<? echo $params["min_--nruns"];?>"/></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>Semilla</strong></td>
      <td class="td_tabladatos_PAR" colspan="2"><input name="--seed" type="text" id="--seed" size="4" value="<? echo $params["--seed"];?>" />
        (integer)</td>
		<td class="td_tabladatos_PAR" colspan="2" ></td>
		<td class="td_tabladatos_PAR" colspan="2" ></td>
    </tr>
    <th class="th_tabladatos" scope="row" colspan="7"><strong>Genetic algorithm params</strong></th>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Population size</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--GA_pop_size" id="--GA_pop_size" type="text" size="4" value="<? echo $params["--population_size"];?>"/>
    		(integer)
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2" ></td>
		<td class="td_tabladatos_PAR" colspan="2" ></td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Intermediate population size</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--GA_ipop_size" id="--GA_ipop_size" type="text" size="4" value="<? echo $params["--intermediate_population_size"];?>"/>
    		(integer)
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2" ></td>
		<td class="td_tabladatos_PAR" colspan="2" ></td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Max time to spend</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--GA_max_time_to_spend" id="--GA_max_time_to_spend" type="text" size="4" value="<? echo $params["--max_time_to_spend"];?>"/>
    		(integer)
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2" >
    		<input name="--GA_MAX_max_time_to_spend" id="--GA_MAX_max_time_to_spend" type="text" size="4" value="<?echo $params["max_--max_time_to_spend"] ?>"/>
		</td>
    	<td class="td_tabladatos_PAR" colspan="2" >
    		<input name="--GA_MIN_max_time_to_spend" id="--GA_MIN_max_time_to_spend" type="text" size="4" value="<?echo $params["min_--max_time_to_spend"] ?>"/>
    	</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Number of evolution steps</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--GA_evolution_steps" id="--GA_evolution_steps" type="text" size="4" value="<? echo $params["--nb_evolution_step"];?>"/>
    		(integer)
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2" >
    		<input name="--GA_MAX_evolution_steps" id="--GA_MAX_evolution_steps" type="text" size="4" value="<? echo $params["max_--nb_evolution_step"];?>"/>
		</td>
    	<td class="td_tabladatos_PAR" colspan="2" >
    		<input name="--GA_MIN_evolution_steps" id="--GA_MIN_evolution_steps" type="text" size="4" value="<? echo $params["min_--nb_evolution_step"];?>"/>
    	</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Extra parameter</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--GA_extra_parameter" id="--GA_extra_parameter" type="text" size="4" value="<? echo $params["--select_extra_parameter"];?>"/>
    		(double)
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2" >
    		<input name="--GA_MAX_extra_parameter" id="--GA_MAX_extra_parameter" type="text" size="4" value="<? echo $params["max_--select_extra_parameter"];?>"/>
		</td>
    	<td class="td_tabladatos_PAR" colspan="2" >
    		<input name="--GA_MIN_extra_parameter" id="--GA_MIN_extra_parameter" type="text" size="4" value="<? echo $params["min_--select_extra_parameter"];?>"/>
    	</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Crossover probability</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--GA_cross_probability" id="--GA_cross_probability" type="text" size="4" value="<? echo $params["--cross_probability"];?>"/>
    		(double)
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2" >
    		<input name="--GA_MAX_cross_probability" id="--GA_MAX_cross_probability" type="text" size="4" value="<? echo $params["max_--cross_probability"];?>"/>
		</td>
    	<td class="td_tabladatos_PAR" colspan="2" >
    		<input name="--GA_MIN_cross_probability" id="--GA_MIN_cross_probability" type="text" size="4" value="<? echo $params["min_--cross_probability"];?>"/>
    	</td>
		
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Crossover extra parameter</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--GA_cross_extra_parameter" id="--GA_cross_extra_parameter" type="text" size="4" value="<? echo $params["--select_cross_extra_parameter"];?>"/>
    		(double)
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2" >
    		<input name="--GA_MAX_cross_extra_parameter" id="--GA_MAX_cross_extra_parameter" type="text" size="4" value="<? echo $params["max_--select_cross_extra_parameter"];?>"/>
		</td>
    	<td class="td_tabladatos_PAR" colspan="2" >
    		<input name="--GA_MIN_cross_extra_parameter" id="--GA_MIN_cross_extra_parameter" type="text" size="4" value="<? echo $params["min_--select_cross_extra_parameter"];?>"/>
    	</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Mutation probability</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--GA_mutate_probability" id="--GA_mutate_probability" type="text" size="4" value="<? echo $params["--mutate_probability"];?>"/>
    		(double)
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2" >
    		<input name="--GA_MAX_mutate_probability" id="--GA_MAX_mutate_probability" type="text" size="4" value="<? echo $params["max_--mutate_probability"];?>"/>
		</td>
    	<td class="td_tabladatos_PAR" colspan="2" >
    		<input name="--GA_MIN_mutate_probability" id="--GA_MIN_mutate_probability" type="text" size="4" value="<? echo $params["min_--mutate_probability"];?>"/>
    	</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Mutation extra parameter</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--GA_mutate_extra_parameter" id="--GA_mutate_extra_parameter" type="text" size="4" value="<? echo $params["--select_mutate_extra_parameter"];?>"/>
    		(double)
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2" >
    		<input name="--GA_MAX_mutate_extra_parameter" id="--GA_MAX_mutate_extra_parameter" type="text" size="4" value="<? echo $params["max_--select_mutate_extra_parameter"];?>"/>
		</td>
    	<td class="td_tabladatos_PAR" colspan="2" >
    		<input name="--GA_MIN_mutate_extra_parameter" id="--GA_MIN_mutate_extra_parameter" type="text" size="4" value="<? echo $params["min_--select_mutate_extra_parameter"];?>"/>
    	</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row" colspan="7"><strong>Hierarchic genetic strategy</strong></th>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Metaepoch length</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS_metaepoch_length" id="--HGS_metaepoch_length" type="text" size="4" value="<? echo $params["--evolution_steps"]; ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS_MAX_metaepoch_length" id="--HGS_MAX_metaepoch_length" type="text" size="4" value="<? echo $params["max_--evolution_steps"]; ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS_MIN_metaepoch_length" id="--HGS_MIN_metaepoch_length" type="text" size="4" value="<? echo $params["min_--evolution_steps"]; ?>"/>
    	</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Number of metaepoch</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS_nb_of_metaepoch" id="--HGS_nb_of_metaepoch" type="text" size="4" value="<? echo $params["--ga_number_metaepochs"]; ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2">
         <input name="--HGS_MAX_nb_of_metaepoch" id="--HGS_MAX_nb_of_metaepoch" type="text" size="4" value="<? echo $params["max_--ga_number_metaepochs"]; ?>"/>
        </td>
    	<td class="td_tabladatos_PAR" colspan="2">
        <input name="--HGS_MIN_nb_of_metaepoch" id="--HGS_MIN_nb_of_metaepoch" type="text" size="4" value="<? echo $params["min_--ga_number_metaepochs"]; ?>"/>
        </td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Max time to spend</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS_max_time_to_spend" id="--HGS_max_time_to_spend" type="text" size="4" value="<? echo $params["--max_time_to_spend"]; ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS_MAX_max_time_to_spend" id="--HGS_MAX_max_time_to_spend" type="text" size="4" value="<? echo $params["max_--max_time_to_spend"]; ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS_MIN_max_time_to_spend" id="--HGS_MIN_max_time_to_spend" type="text" size="4" value="<? echo $params["min_--max_time_to_spend"]; ?>"/>
    	</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Extra parameter</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS_extra_parameter" id="--HGS_extra_parameter" type="text" size="4" value="<? echo $params["--select_extra_parameter"]; ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS_MAX_extra_parameter" id="--HGS_MAX_extra_parameter" type="text" size="4" value="<? echo $params["max_--select_extra_parameter"]; ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS_MIN_extra_parameter" id="--HGS_MIN_extra_parameter" type="text" size="4" value="<? echo $params["min_--select_extra_parameter"]; ?>"/>
    	</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Crossover extra parameter</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS_cross_extra_param" id="--HGS_cross_extra_param" type="text" size="4" value="<? echo $params["--cross_extra_parameter"]; ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS_MAX_cross_extra_parameter" id="--HGS_MAX_cross_extra_parameter" type="text" size="4" value="<? echo $params["max_--cross_extra_parameter"]; ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS_MIN_cross_extra_parameter" id="--HGS_MIN_cross_extra_parameter" type="text" size="4" value="<? echo $params["min_--cross_extra_parameter"]; ?>"/>
    	</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Mutation extra parameter</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS_mutate_extra_param" id="--HGS_mutate_extra_param" type="text" size="4" value="<? echo $params["--mutate_extra_parameter"]; ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS_MAX_mutate_extra_parameter" id="--HGS_MAX_mutate_extra_parameter" type="text" size="4" value="<? echo $params["max_--mutate_extra_parameter"]; ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS_MIN_mutate_extra_parameter" id="--HGS_MIN_mutate_extra_parameter" type="text" size="4" value="<? echo $params["min_--mutate_extra_parameter"]; ?>"/>
    	</td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Enviroment length</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS_env_length" id="--HGS_env_length" type="text" size="4" value="<? echo $params["--enviroment_length"]; ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS_MAX_env_length" id="--HGS_MAX_env_length" type="text" size="4" value="<?echo $params["max_--enviroment_length"]; ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS_MIN_env_length" id="--HGS_MIN_env_length" type="text" size="4" value="<?echo $params["min_--enviroment_length"]; ?>"/></td>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row" colspan="7"><strong>Settings for root</strong></th>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Population size</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS2_pop_size" id="--HGS2_pop_size" type="text" size="4" value="<? echo $params["--hgs_pop_size"] ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>    	
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Intermediate population size</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS2_ipop_size" id="--HGS2_ipop_size" type="text" size="4" value="<? echo $params["--hgs_intermediate_size"] ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>    	
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Crossover probability</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS2_cross_probab" id="--HGS2_cross_probab" type="text" size="4" value="<? echo $params["--hgs_prob_cross"] ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS2_MAX_cross_probab" id="--HGS2_MAX_cross_probab" type="text" size="4" value="<? echo $params["max_--hgs_prob_cross"];?>"/>
    	</td> 
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS2_MIN_cross_probab" id="--HGS2_MIN_cross_probab" type="text" size="4" value="<? echo $params["min_--hgs_prob_cross"];?>"/>
    	</td>   	
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Mutation probability</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS2_mutate_probab" id="--HGS2_mutate_probab" type="text" size="4" value="<? echo $params["--hgs_prob_mutate"] ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS2_MAX_mutate_probab" id="--HGS2_MAX_mutate_probab" type="text" size="4" value="<? echo $params["max_--hgs_prob_mutate"];?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--HGS2_MIN_mutate_probab" id="--HGS2_MIN_mutate_probab" type="text" size="4" value="<? echo $params["min_--hgs_prob_mutate"];?>"/>
    	</td>    	
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row" colspan="7"><sttrong>Tabu search parameters</sttrong></th>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Number iterations</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--TS_nb_iterations" id="--TS_nb_iterations" type="text" size="4" value="<? echo $params["--tb_nb_iterations"] ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>    	
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Tabu size</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--TS_tabu_size" id="--TS_tabu_size" type="text" size="4" value="<? echo $params["--tb_tabu_size"] ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>    	
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Max tabu status</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--TS_max_tabu_status" id="--TS_max_tabu_status" type="text" size="4" value="<? echo $params["--tb_max_tabu_status"] ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>    	
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Min tabu status</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--TS_min_tabu_status" id="--TS_min_tabu_status" type="text" size="4" value="<? echo $params["--tb_min_tabu_status"] ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>    	
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Max repetitions</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--TS_max_repetitions" id="--TS_max_repetitions" type="text" size="4" value="<? echo $params["--tb_max_repetitions"] ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>    	
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Number of intensifications</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--TS_nb_intensifications" id="--TS_nb_intensifications" type="text" size="4" value="<? echo $params["--tb_nb_intensifications"] ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>    	
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Number of diversifications</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--TS_nb_diversifications" id="--TS_nb_diversifications" type="text" size="4" value="<? echo $params["--tb_nb_diversifications"] ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>    	
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Elite size</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--TS_elite_size" id="--TS_elite_size" type="text" size="4" value="<? echo $params["--tb_elite_size"] ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>    	
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Aspiration value</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--TS_aspiration_value" id="--TS_aspiration_value" type="text" size="4" value="<? echo $params["--tb_aspiration_value"] ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>    	
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Max time to spend</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--TS_max_time_to_spend" id="--TS_max_time_to_spend" type="text" size="4" value="<? echo $params["--max_time_to_spend"] ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--TS_MAX_max_time_to_spend" id="--TS_MAX_max_time_to_spend" type="text" size="4" value="<? echo $params["max_--max_time_to_spend"] ?>"/>
    	</td> 
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--TS_MIN_max_time_to_spend" id="--TS_MIN_max_time_to_spend" type="text" size="4" value="<? echo $params["min_--max_time_to_spend"] ?>"/>
    	</td> 	
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row" colspan="7"><strong>Game-based scheduler parameters</strong></th>
    </tr>
    <tr>
    	<th class="th_tabladatos" scope="row"><strong>Number of players</strong></th>
    	<td class="td_tabladatos_PAR" colspan="2">
    		<input name="--SG_nb_players" id="--SG_nb_players" type="text" size="4" value="<? echo $params["--number_players"]; ?>"/>
    	</td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>
    	<td class="td_tabladatos_PAR" colspan="2"></td>
    </tr>
  </table>
  <br />
  <input type="submit" name="Submit" value="Guardar" />
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
