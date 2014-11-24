<?
	include "../../lib/Graph.php";
	include "../config.php";
	include "../../management/userManager.php";
	require_once "../../lib/phplot/phplot.php";
	
	function buildOcupationGraph()
	{
		$userm = new userManager();
		$info['data'] = $userm->getUsersByGroup("ocupacion");
		$info['title'] = "Distribucion ocupacion usuario";
		$info['data_type'] = "text-data-single";
		$info['graph_type'] = "pie";
		$info['is_legend'] = true;
		$info['legend'] = array();
		
		foreach ($info["data"] as $row)
  			array_push($info["legend"],implode(': ', $row));
		
		$graph = new Graph($info,450,250);
		$graph->draw();
	}
	function buildCountriesGraph()
	{
		$userm = new userManager();
		$info['data'] = $userm->getUsersByGroup("pais");
		$info['title'] = "Distribucion paises usuario";
		$info['data_type'] = "text-data-single";
		$info['graph_type'] = "pie";
		$info['is_legend'] = true;
		$info['legend'] = array();
		
		foreach ($info["data"] as $row)
  			array_push($info["legend"],implode(': ', $row));
		
		$graph = new Graph($info,450,250);
		$graph->draw();
	}
	
	function buildUsersEvolutionGraph($month_ini,$year_ini,$month_fin, $year_fin)
	{
		$meses = array("","enero","febrero","marzo","abril","mayo","junio","julio","agosto","septiembre","octubre","noviembre","diciembre");
		$info['data'] = array();
		$userm = new userManager();
		$order["order"] = "username";
		$order["way"] = "ASC";
		
		if($year_ini == $year_fin)
		{
			$diff = $month_fin- $month_ini + 1;
		}
		else
		{
			if ($year_fin == $year_ini + 1) 
			{
				$diff = (12 - $month_ini + 1) + $month_fin;
			}
			else if($year_fin > $year_ini +1)
			{
				$diff = ($year_fin - $year_ini - 1)*12 + (12 - $month_ini + 1) + $month_fin; 
			}
		}
	
		$filter["fecha_alta"]["ini"] = 0;
		for( $i = 0; $i < $diff; $i++)
		{
			
			$filter["fecha_alta"]["fin"] = mktime(0,0,0,$month_ini+1,0,$year_ini);
			
			//echo "<br> ini:".date("d/m/Y",$filter["fecha_alta"]["ini"])."<br>";
			//echo "<br> fin:".date("d/m/Y",$filter["fecha_alta"]["fin"])."<br>";			
			$users = $userm->getUsers($filter,$order);
			$data = array($meses[$month_ini]."-".$year_ini,count($users));
			array_push($info['data'],$data);
			$month_ini++;
			if($month_ini >12)
			{
				$month_ini = 1;
				$year_ini++;
			}
		}
		
		$info['title_Y'] = "numero usuarios(entero)";
		$info['showYLabel'] = true;
		$info['YTickIncrement'] = 2;
		$info['title_X'] = "meses";
		$info['title'] = "Evolucion usuarios";
		$info['data_type'] = "text-data";
		$info['graph_type'] = "linepoints";
		$info['is_legend'] = false;
		$graph = new Graph($info,600,400);
		$graph->draw();
		 
	}
	
	
	if(isset($_GET["graph"]))
	{
		switch($_GET["graph"])
		{
			case "countries":
								buildCountriesGraph();
								break;
			case "ocupation":	
								buildOcupationGraph();
								break;
			case "users_evolution":
									buildUsersEvolutionGraph($_GET["month_ini"], $_GET["year_ini"],$_GET["month_fin"],$_GET["year_fin"]);
									break;															
		}		
	}
	
?>
