<?
	include 'phplot/phplot.php';
	class Graph
	{
		var $graph;
		var $info;
		
		function Graph($data,$width,$height)
		{
			$this->graph = new PHPlot($width,$height);
			$this->info = $data;
			//print_r($data);	
		}
		
		function draw()
		{			
			
			$this->graph->SetImageBorderType('plain');
			$this->graph->SetPlotType($this->info['graph_type']);
			$this->graph->SetDataValues($this->info['data']);//($this->info['data']);
			$this->graph->SetDataType($this->info['data_type']);//($this->info['data_type']);
			//formateo de la gráfica
				//$this->graph->SetPlotType($this->info['graph_type']);
				//$this->graph->SetVertTickIncrement($this->info['tick_inc']);
				//$this->graph->SetTickLength($this->info['tick_length']);
				//$this->graph->SetTickColor($this->info['tick_color']);
				//$this->graph->SetPointShape($this->info['point_shape']);
				//$this->graph->SetPointSize($this->info['point_size']);
				//$this->graph->SetTitle($this->info['title_graph']);
			$this->graph->SetXLabel($this->info['title_X']);
			$this->graph->SetYLabel($this->info['title_Y']);
			if($this->info["is_legend"] == true)
				$this->graph->SetLegend($this->info['legend']);
			
			$this->graph->SetTitle($this->info["title"]);
			/*if($this->info["showXLabel"] == false || $this->info["showXLabel"] == "")
			{
				$this->graph->SetXTickLabelPos('none');
				$this->graph->SetXTickPos('none');
			}
			
			if($this->info["showYLabel"] == false || $this->info["showYLabel"] == "")
			{
				$this->graph->SetYTickLabelPos('none');
				$this->graph->SetYTickPos('none');
			}
			
			if($this->info["XTickIncrement"] != "")
				$this->graph->SetXTickIncrement($this->info["XTickIncrement"]);
			
			if($this->info["YTickIncrement"] != "")
				$this->graph->SetYTickIncrement($this->info["YTickIncrement"]);
			
			/*if($this->info["YTickIncrement"] != "")
				$this->graph->SetYTickIncrement($this->info["YTickIncrement"]);
			else	
				$this->graph->SetYTickIncrement(1);*/
			//$this->graph->SetLegendPixels($this->info['legend_X'],$this->info['legend_Y']);
			//$this->graph->SetLineWidth($this->info['line_width']);
			//$this->graph->SetLineStyles($this->info['line_style']);
			//$this->graph->SetShading($this->info['line_shadow']);
			//$this->graph->SetBackgroundColor($this->info['bg_color']);
			//$this->graph->SetGridColor($this->info['grid_color']);
			//$this->graph->SetDataColors($this->info['data_colors']);
			//$this->graph->SetTextColor($this->info['scale_color']);
			//$this->graph->SetTitleColor($this->info['title_color']);
			//$this->graph->SetLightGridColor($this->info['grid_color']);
			   //Visualización de la gráfica
			$this->graph->DrawGraph();
		}			
	}
?>
