<?
	include "../../lib/Graph.php";
	include "../config.php";
	include "../../management/executionManager.php";
	require_once "../../lib/phplot/phplot.php";
	
	function buildStatusGraph()
	{
		$exem = new executionManager();
		$info['data'] = $exem->getExecutionsByGroup("status");
		$info['title'] = "Status de las ejecuciones";
		$info['data_type'] = "text-data-single";
		$info['graph_type'] = "pie";
		$info['is_legend'] = true;
		$info['legend'] = array();
		
		foreach ($info["data"] as $row)
  			array_push($info["legend"],implode(': ', $row));
		
		$graph = new Graph($info,450,250);
		$graph->draw();
	}
	
	
	function buildExecutionsIntervalGraph($month_ini,$year_ini,$month_fin, $year_fin)
	{
		$meses = array("","enero","febrero","marzo","abril","mayo","junio","julio","agosto","septiembre","octubre","noviembre","diciembre");
		$info['data'] = array();
		$exem = new executionManager();
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
	
		
		for( $i = 0; $i < $diff; $i++)
		{
			
			$filter["fecha"]["ini"] = mktime(0,0,0,$month_ini,1,$year_ini);
			$filter["fecha"]["fin"] = mktime(0,0,0,$month_ini+1,0,$year_ini);
			$filter["status"] = "finished"; 
			//echo "<br> ini:".date("d/m/Y",$filter["fecha_alta"]["ini"])."<br>";
			//echo "<br> fin:".date("d/m/Y",$filter["fecha_alta"]["fin"])."<br>";			
			$execfinished = $exem->getExecutions($filter,$order);
			$filter["status"] = "running"; 
			$execrunning = $exem->getExecutions($filter,$order);
			
			$data = array($meses[$month_ini]."-".$year_ini,count($execfinished),count($execrunning));
			array_push($info['data'],$data);
			$month_ini++;
			if($month_ini >12)
			{
				$month_ini = 1;
				$year_ini++;
			}
		}
		$info['title'] = "numero de ejecuciones";
		$info['title_Y'] = "numero ejecuciones(entero)";
		$info['YTickIncrement'] = 5;
		$info['title_X'] = "meses";
		$info['showYLabel'] = true;
		$info['data_type'] = "text-data";
		$info['graph_type'] = "stackedbars";
		$info['is_legend'] = true;
		$info['legend'] = array("finished","running");
		$graph = new Graph($info,600,400);
		$graph->draw();
	}
	
	
	if(isset($_GET["graph"]))
	{
		switch($_GET["graph"])
		{
			case "status":
								buildStatusGraph();
								break;
			case "executions_interval":	
								buildExecutionsIntervalGraph($_GET["month_ini"], $_GET["year_ini"],$_GET["month_fin"],$_GET["year_fin"]);
								break;											
		}		
	}
	
?>
