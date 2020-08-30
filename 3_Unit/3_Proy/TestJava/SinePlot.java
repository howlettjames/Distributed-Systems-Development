import javafx.application.Application;
import javafx.scene.Scene;
import javafx.stage.Stage;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.geometry.Orientation;
import javafx.scene.layout.Pane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import java.util.List;
import java.util.ArrayList;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.scene.text.Text;
import javafx.scene.paint.Color;
import javafx.event.EventHandler;
import javafx.event.ActionEvent;
import javafx.scene.input.KeyCode;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;
import javafx.scene.chart.LineChart;
import javafx.animation.Timeline;
import javafx.animation.KeyFrame;
import javafx.util.Duration;

public class SinePlot extends Application
{
	private XYChart.Series<Number, Number> serieFourier1;     
	private XYChart.Series<Number, Number> serieFourier2;     
	NumberAxis xAxis = new NumberAxis(-7, 7, 1);
    NumberAxis yAxis = new NumberAxis(-7, 7, 1);      
    LineChart<Number,Number> lineChart = new LineChart<>(xAxis, yAxis);
    double n = 1.0;	
    double a0 = 1.5;
    double bn = 0.0;
    double[] sum_points = new double[1257];
					
	@Override
	public void start(Stage primaryStage)
	{
		// PANEL PRINCIPAL EN EL QUE SE DESPLEGARÁ EL LINECHART
		VBox fourierPane = new VBox(20);
		
		// PONEMOS LAS ETIQUETAS PARA LOS EJES Y LA PROPIEDAD DE QUE SE AUTOCENTREN CADA QUE CAMBIEN LAS COORDENADAS
        xAxis.setLabel("t");                
        yAxis.setLabel("f(t)");
        // xAxis.setAutoRanging(true);
        // yAxis.setAutoRanging(true);

        // COLOCAMOS PADDING AL PANEL
        fourierPane.setPadding(new Insets(5));
        
		// VACIAMOS EL LINECHART 
		// lineChart.getData().remove(0);
        
		// COLOCAMOS EL PRIMER TERMINO DE LA SERIE
		serieFourier1 = new XYChart.Series<>();
		serieFourier1.setName("Serie de Fourier 1");	
		serieFourier2 = new XYChart.Series<>();
		serieFourier2.setName("Serie de Fourier 2");	
		
		int npoint = 0;	
		for(double t = - 2 * Math.PI; t <= 2 * Math.PI; t += 0.01, npoint++)
		{
			sum_points[npoint] = a0;
			serieFourier1.getData().add(new XYChart.Data<>(t, sum_points[npoint]));
			serieFourier2.getData().add(new XYChart.Data<>(t, sum_points[npoint]));
		}

        // AÑADIMOS LA SERIE DE FOURIER AL LINECHART PARA SER DESPLEGADO
        lineChart.getData().add(serieFourier1);
        lineChart.getData().add(serieFourier2);
        
        // QUITAMOS LOS PEQUEÑOS CIRCULOS QUE SE DIBUJAN POR DEFAULT EN CADA PUNTO, PARA QUE SE VEA MÁS COMO UNA LÍNEA
        lineChart.setCreateSymbols(false);

        // AÑADIMOS EL LINECHART AL PANEL PRINCIPAL
		fourierPane.getChildren().addAll(lineChart);
        
        // PROGRAMAMOS UN TIMELINE PARA QUE CADA SEGUNDO HAGA UNA ACTUALIZACIÓN DE LA SERIE DE FOURIER
		Timeline fiveSecondsWonder = new Timeline(new KeyFrame(Duration.seconds(1), new EventHandler<ActionEvent>() 
		{
		    @Override
		    public void handle(ActionEvent event) 
		    {
					// CALCULAMOS bn PORQUE ESTA SERIE SOLO TIENE ESTE TÉRMINO
					bn = ( -3.0 / (n * Math.PI) * (Math.cos(n * Math.PI) - 1) );
					
					// SI EL VALOR DE bn ES CERO O NaN NO ES NECESARIO HACER TODO EL CÁLCULO Y SE DEJA COMO ESTÁ
					if(!Double.isNaN(bn) && bn != 0)
					{		
						// VACÍAMOS EL LINECHART
						lineChart.getData().remove(0);
						
						// PREPARAMOS UNA NUEVA SERIE DE PUNTOS
						serieFourier1 = new XYChart.Series<>();
						serieFourier1.setName("Serie de Fourier");	
						
						// HACEMOS EL CÁLCULO DEL ENEAVO TÉRMINO ACTUAL DE LA SERIE DE FOURIER
						int npoint = 0;	
						for(double t = -2 * Math.PI; t <= 2 * Math.PI; t += 0.01, npoint++) 
						{
							// LA SUMA DE LOS TÉRMINOS POR CADA PUNTO ES ALMACENADA EN EL ARREGLO DE PUNTOS "SUM_POINTS"
							sum_points[npoint] += bn * Math.sin(t * n);
							// AGREGAMOS EL PUNTO EN CURSO A LA SERIE DE PUNTOS QUE SE VAN A VISUALIZAR EN EL CHART
				         	serieFourier1.getData().add(new XYChart.Data<>(t, sum_points[npoint]));
				        }
				        // POR ÚLTIMO AGREGAMOS DICHA SERIE DE PUNTOS AL CHART
				        lineChart.getData().add(serieFourier1);
					}
					// INCREMENTAMOS EL TÉRMINO DE LA SERIE DE FOURIER			        
			        n++;
		    }
		}));

		fiveSecondsWonder.setCycleCount(Timeline.INDEFINITE);
		fiveSecondsWonder.play();

		// POR ÚLTIMO AÑADIMOS UN ESCENARIO QUE CONTENDRÁ AL PANEL PRINCIPAL Y QUE PONDREMOS EN EL STAGE PRINCIPAL
		Scene scene = new Scene(fourierPane, 800, 600);
		primaryStage.setScene(scene);
		primaryStage.setTitle("SERIE DE FOURIER DE UNA SEÑAL PERIÓDICA");			
		primaryStage.show();

/*		new Timer().schedule(new TimerTask() 
		{
	        @Override
	        public void run() 
	        {
	            System.out.println("ping");
        	}
    	}, 0, 5000);
*/
	}	
}