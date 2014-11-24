<?
	include "../../lib/Graph.php";
	include "../config.php";
	include "../../management/userManager.php";
	require_once "../../lib/phplot/phplot.php";
	
	
	function buildSessionExecGraph($username,$month_ini,$year_ini,$month_fin, $year_fin)
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
	
		
		for( $i = 0; $i < $diff; $i++)
		{
			
			$filter["ini"] = mktime(0,0,0,$month_ini,1,$year_ini);
			$filter["fin"] = mktime(0,0,0,$month_ini+1,0,$year_ini);
			
			//echo "<br> ini:".date("d/m/Y",$filter["fecha_alta"]["ini"])."<br>";
			//echo "<br> fin:".date("d/m/Y",$filter["fecha_alta"]["fin"])."<br>";			
			$num_exec = $userm->getNumberExecutions($filter["ini"],$filter["fin"],$username);
			$num_ses = $userm->getNumberSessions($filter["ini"],$filter["fin"],$username);
			$data = array($meses[$month_ini]."-".$year_ini,$num_exec,$num_ses);
			array_push($info['data'],$data);
			$month_ini++;
			if($month_ini >12)
			{
				$month_ini = 1;
				$year_ini++;
			}
		}
		$info['title_Y'] = "numero ejecuciones(entero) / sesiones(entero)";
		$info['showYLabel'] = true;
		$info['YTickIncrement'] = 2;
		$info['title_X'] = "meses";
		$info['title'] = "Ejecuciones y sesiones del usuario";
		$info['data_type'] = "text-data";
		$info['graph_type'] = "bars";
		$info['is_legend'] = true;
		$info['legend'] = array("ejecuciones","sesiones");
		$graph = new Graph($info,600,400);
		$graph->draw();
		 
	}
	
	
	if(isset($_GET["graph"]))
	{
		switch($_GET["graph"])
		{
			case "session_exec":
									buildSessionExecGraph($_GET["username"],$_GET["month_ini"], $_GET["year_ini"],$_GET["month_fin"],$_GET["year_fin"]);
									break;															
		}		
	}
	
?>
