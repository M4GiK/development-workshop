
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Documento sin t&iacute;tulo</title>
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
	
	include "configManager.php";
	include "../config.php";
	
	$cm = new configManager();
	$params = $cm->getDefaultParams();
?>
<h1>Parametros simulador</h1>
<form id="form1" name="form1" method="post" action="saveParams.php">
  <table width="800" cellspacing="0">
    <tr>
      <td width="337" align="center" bgcolor="#FFFFCC"><strong>Numero de tareas </strong></td>
      <td colspan="2"><input name="--ttasks" type="text" id="--ttasks" size="4" value="<? echo $params["--ttasks"];?>"/>
        (integer)</td>
    </tr>
    <tr>
      <td align="center" bgcolor="#FFFF99"><strong>Numero inicial de tareas </strong></td>
      <td colspan="2"><input name="--itasks" type="text" id="--itasks" size="4" value="<? echo $params["--itasks"];?>"/>
        (integer)</td>
    </tr>
    <tr>
      <td align="center" bgcolor="#FFFFCC"><strong>Distribucion llegadas al sistema </strong></td>
      <td width="162"><select name="--iatime" id="--iatime" onchange="loadProperties('params_iatime',this)"; >
	  	  <? getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$params["--iatime"]);?>
		  
		  </select>      </td>
	   <td width="293">
	   <div id="params_iatime">
	   	<?
			getProperties("--iatime",$params["--iatime"],$params);
		?>
	   </div>
	   </td>
    </tr>
    <tr>
      <td align="center" bgcolor="#FFFF99"><strong>Distribucion carga trabajo </strong></td>
      <td ><select name="--workload" id="--workload" onchange="loadProperties('params_workload',this)";>
	  		<? getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$params["--workload"]);?>
						</select>        </td>
	<td width="293"><div id="params_workload" >	<?
			getProperties("--workload",$params["--workload"],$params);
		?></div></td>	
    </tr>
    <tr>
      <td align="center" bgcolor="#FFFFCC"><strong>Numero inicial maquinas entorno </strong></td>
      <td colspan="2"><input name="--ihosts" type="text" id="--ihosts" size="4" value="<? echo $params["--ihosts"];?>" />
        (integer)</td>
    </tr>
    <tr>
      <td align="center" bgcolor="#FFFF99"><strong>Distribucion mips </strong></td>
      <td ><select name="--mips" id="--mips" onchange="loadProperties('params_mips',this)">
	  				  <? getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$params["--mips"]);?>
					</select>      </td>
		<td width="293"><div id="params_mips" >	<?
			getProperties("--mips",$params["--mips"],$params);
		?></div></td>
    </tr>
    <tr>
      <td align="center" bgcolor="#FFFFCC"><strong>Distribucion intervalo tiempo alta recursos </strong></td>
      <td ><select name="--addhost" id="--addhost" onchange="loadProperties('params_addhost',this)">
	  				<? getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$params["--addhost"]);?>
						</select>        </td>
		<td width="293"><div id="params_addhost" >	<?
			getProperties("--addhost",$params["--addhost"],$params);
		?></div></td>
    </tr>
    <tr>
      <td align="center" bgcolor="#FFFF99"><strong>Distribucion intervalo de tiempo baja recursos </strong></td>
      <td><select name="--delhost" id="--delhost" onchange="loadProperties('params_delhost',this)"><option value="c">constant</option>
		  <? getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$params["--delhost"]);?>      
		  </td>
		<td width="293"><div id="params_delhost" > 	<?
			getProperties("--delhost",$params["--delhost"],$params);
		?></div></td>
   </tr>
   <tr>
      <td align="center" bgcolor="#FFFFCC"><strong>Minimo numero de maquinas sistema </strong></td>
      <td colspan="2"><input name="--minhosts" type="text" id="--minhosts" size="4" value="<? echo $params["--minhosts"];?>" />
        (integer)</td>
    </tr>
    <tr>
      <td align="center" bgcolor="#FFFF99"><strong>Maximo numero de maquinas del sistema </strong></td>
      <td colspan="2"><input name="--maxhosts" type="text" id="--maxhosts" size="4" value="<? echo $params["--maxhosts"];?>"/>
        (integer)</td>
    </tr>
    <tr>
      <td align="center" bgcolor="#FFFFCC"><strong>Activar replanificacion </strong></td>
      <td colspan="2"><select name="--reschedule" id="--reschedule" >
	  	<? getOptions(array("yes","no"),array("Y","N"),$params["--reschedule"]);?>
	  </select></td>
    </tr>
    <tr>
      <td align="center" bgcolor="#FFFF99"><strong>Politica seleccion recursos </strong></td>
      <td colspan="2"><select name="--hostselect" id="--hostselect" >
	  	<? getOptions(array("idle","all"),array("idle","all"),$params["--hostselect"]);?>
	  </select></td>
    </tr>
    <tr>
      <td align="center" bgcolor="#FFFFCC"><strong>Politica activacion planificador </strong></td>
      <td ><select name="--activate" id="--activate" onchange="loadProperties('params_activate',this)" >
	<? getOptions(array("constant","uniform","exponential","zipf","triangle","normal"),array("c","u","e","zipf","triangle","n"),$params["--activate"]);?>      
	</td>
	  <td width="293"><div id="params_activate" >	<?
			getProperties("--activate",$params["--activate"],$params);
		?></div></td>
   	</tr>
   	<tr>
      <td align="center" bgcolor="#FFFF99"><strong>Politica local gestion tareas de cada host </strong></td>
    	 <td colspan="2"><select name="--allocpolicy" id="--allocpolicy" >
	<? getOptions(array("fcfs","estf"),array("fcfs","estf"),$params["--allocpolicy"]);?>
	  </select></td>
    </tr>
    <tr>
      <td align="center" bgcolor="#FFFFCC"><strong>Ejecuciones independientes </strong></td>
      <td colspan="2"><input name="--nruns" type="text" id="--nruns" size="4" value="<? echo $params["--nruns"];?>"/>
      (integer)</td>
    </tr>
    <tr>
      <td align="center" bgcolor="#FFFF99"><strong>Semilla</strong></td>
      <td colspan="2"><input name="--seed" type="text" id="--seed" size="4" value="<? echo $params["--seed"];?>" />
      (integer)</td>
    </tr>
  </table>
  <br />
  <input type="submit" name="Submit" value="Guardar" />
</form>
<br />
<a href="index.php">volver
</a>
</body><br />
<br />

</html>
