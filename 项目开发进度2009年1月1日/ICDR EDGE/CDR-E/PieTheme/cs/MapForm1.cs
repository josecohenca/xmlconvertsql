using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.IO;
using System.Reflection;
using MapInfo.Data;
using MapInfo.Mapping;
using MapInfo.Engine;
using MapInfo.Windows.Dialogs;
using MapInfo.Mapping.Thematics;

namespace PieThemeSample
{
	/// <summary>
	/// Summary description for MapForm1.
	/// </summary>
	public class MapForm1 : System.Windows.Forms.Form
	{
		private MapInfo.Windows.Controls.MapControl mapControl1;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.StatusBar statusBar1;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.CheckedListBox fields;
		private System.Windows.Forms.ComboBox layers;
		private System.Windows.Forms.Label label2;
		internal MapInfo.Windows.Controls.MapToolBar mapToolBar1;
		internal MapInfo.Windows.Controls.MapToolBarButton mapToolBarButtonOpenTable;
		private MapInfo.Windows.Controls.MapToolBarButton mapToolBarButtonLayerControl;
		internal System.Windows.Forms.ToolBarButton toolBarButtonSeparator;
		internal MapInfo.Windows.Controls.MapToolBarButton mapToolBarButtonSelect;
		internal MapInfo.Windows.Controls.MapToolBarButton mapToolBarButtonZoomIn;
		internal MapInfo.Windows.Controls.MapToolBarButton mapToolBarButtonZoomOut;
		internal MapInfo.Windows.Controls.MapToolBarButton mapToolBarButtonPan;
		private System.ComponentModel.Container components = null;

		public MapForm1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			// Listen to the right map event to allow the status bar Zoom level to be updated
			mapControl1.Map.ViewChangedEvent += new ViewChangedEventHandler(Map_ViewChanged);
	}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.mapControl1 = new MapInfo.Windows.Controls.MapControl();
			this.panel1 = new System.Windows.Forms.Panel();
			this.statusBar1 = new System.Windows.Forms.StatusBar();
			this.label1 = new System.Windows.Forms.Label();
			this.fields = new System.Windows.Forms.CheckedListBox();
			this.layers = new System.Windows.Forms.ComboBox();
			this.label2 = new System.Windows.Forms.Label();
			this.mapToolBar1 = new MapInfo.Windows.Controls.MapToolBar();
			this.mapToolBarButtonOpenTable = new MapInfo.Windows.Controls.MapToolBarButton();
			this.mapToolBarButtonLayerControl = new MapInfo.Windows.Controls.MapToolBarButton();
			this.toolBarButtonSeparator = new System.Windows.Forms.ToolBarButton();
			this.mapToolBarButtonSelect = new MapInfo.Windows.Controls.MapToolBarButton();
			this.mapToolBarButtonZoomIn = new MapInfo.Windows.Controls.MapToolBarButton();
			this.mapToolBarButtonZoomOut = new MapInfo.Windows.Controls.MapToolBarButton();
			this.mapToolBarButtonPan = new MapInfo.Windows.Controls.MapToolBarButton();
			this.panel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// mapControl1
			// 
			this.mapControl1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.mapControl1.Location = new System.Drawing.Point(0, 0);
			this.mapControl1.Name = "mapControl1";
			this.mapControl1.Size = new System.Drawing.Size(296, 244);
			this.mapControl1.TabIndex = 0;
			this.mapControl1.Text = "mapControl1";
			this.mapControl1.Tools.MiddleButtonTool = null;
			// 
			// panel1
			// 
			this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.panel1.Controls.Add(this.mapControl1);
			this.panel1.Location = new System.Drawing.Point(4, 40);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(300, 248);
			this.panel1.TabIndex = 1;
			// 
			// statusBar1
			// 
			this.statusBar1.Location = new System.Drawing.Point(0, 291);
			this.statusBar1.Name = "statusBar1";
			this.statusBar1.Size = new System.Drawing.Size(520, 19);
			this.statusBar1.TabIndex = 2;
			// 
			// label1
			// 
			this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label1.Location = new System.Drawing.Point(320, 38);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(168, 16);
			this.label1.TabIndex = 5;
			this.label1.Text = "Layers:";
			// 
			// fields
			// 
			this.fields.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.fields.Location = new System.Drawing.Point(320, 94);
			this.fields.Name = "fields";
			this.fields.Size = new System.Drawing.Size(184, 184);
			this.fields.TabIndex = 6;
			this.fields.SelectedIndexChanged += new System.EventHandler(this.fields_SelectedIndexChanged);
			// 
			// layers
			// 
			this.layers.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.layers.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.layers.Location = new System.Drawing.Point(320, 54);
			this.layers.Name = "layers";
			this.layers.Size = new System.Drawing.Size(184, 21);
			this.layers.TabIndex = 7;
			// 
			// label2
			// 
			this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label2.Location = new System.Drawing.Point(320, 78);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(168, 16);
			this.label2.TabIndex = 8;
			this.label2.Text = "Fields:";
			// 
			// mapToolBar1
			// 
			this.mapToolBar1.Buttons.AddRange(new System.Windows.Forms.ToolBarButton[] {
																						   this.mapToolBarButtonOpenTable,
																						   this.mapToolBarButtonLayerControl,
																						   this.toolBarButtonSeparator,
																						   this.mapToolBarButtonSelect,
																						   this.mapToolBarButtonZoomIn,
																						   this.mapToolBarButtonZoomOut,
																						   this.mapToolBarButtonPan});
			this.mapToolBar1.Divider = false;
			this.mapToolBar1.Dock = System.Windows.Forms.DockStyle.None;
			this.mapToolBar1.DropDownArrows = true;
			this.mapToolBar1.Location = new System.Drawing.Point(6, 8);
			this.mapToolBar1.MapControl = this.mapControl1;
			this.mapToolBar1.Name = "mapToolBar1";
			this.mapToolBar1.ShowToolTips = true;
			this.mapToolBar1.Size = new System.Drawing.Size(160, 26);
			this.mapToolBar1.TabIndex = 9;
			// 
			// mapToolBarButtonOpenTable
			// 
			this.mapToolBarButtonOpenTable.ButtonType = MapInfo.Windows.Controls.MapToolButtonType.OpenTable;
			this.mapToolBarButtonOpenTable.ToolTipText = "Open Table";
			// 
			// mapToolBarButtonLayerControl
			// 
			this.mapToolBarButtonLayerControl.ButtonType = MapInfo.Windows.Controls.MapToolButtonType.LayerControl;
			this.mapToolBarButtonLayerControl.ToolTipText = "Layer Control";
			// 
			// toolBarButtonSeparator
			// 
			this.toolBarButtonSeparator.Style = System.Windows.Forms.ToolBarButtonStyle.Separator;
			// 
			// mapToolBarButtonSelect
			// 
			this.mapToolBarButtonSelect.ButtonType = MapInfo.Windows.Controls.MapToolButtonType.Select;
			this.mapToolBarButtonSelect.ToolTipText = "Select";
			// 
			// mapToolBarButtonZoomIn
			// 
			this.mapToolBarButtonZoomIn.ButtonType = MapInfo.Windows.Controls.MapToolButtonType.ZoomIn;
			this.mapToolBarButtonZoomIn.ToolTipText = "Zoom-in";
			// 
			// mapToolBarButtonZoomOut
			// 
			this.mapToolBarButtonZoomOut.ButtonType = MapInfo.Windows.Controls.MapToolButtonType.ZoomOut;
			this.mapToolBarButtonZoomOut.ToolTipText = "Zoom-out";
			// 
			// mapToolBarButtonPan
			// 
			this.mapToolBarButtonPan.ButtonType = MapInfo.Windows.Controls.MapToolButtonType.Pan;
			this.mapToolBarButtonPan.ToolTipText = "Pan";
			// 
			// MapForm1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(520, 310);
			this.Controls.Add(this.mapToolBar1);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.layers);
			this.Controls.Add(this.fields);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.statusBar1);
			this.Controls.Add(this.panel1);
			this.MinimumSize = new System.Drawing.Size(250, 200);
			this.Name = "MapForm1";
			this.Text = "Pie Theme Sample";
			this.Load += new System.EventHandler(this.MapForm1_Load);
			this.panel1.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new MapForm1());
		}

		// Handler function called when the active map's view changes
		private void Map_ViewChanged(object o, ViewChangedEventArgs e) {
			// Get the map
			MapInfo.Mapping.Map map = (MapInfo.Mapping.Map) o;
			// Display the zoom level
			Double dblZoom = System.Convert.ToDouble(String.Format("{0:E2}", mapControl1.Map.Zoom.Value));
			statusBar1.Text = "Zoom: " + dblZoom.ToString() + " " + mapControl1.Map.Zoom.Unit.ToString();
		}

		private void MapForm1_Load(object sender, System.EventArgs e)
		{
			// Set table search path to value sampledatasearch registry key
			// if not found, then just use the app's current directory
			Microsoft.Win32.RegistryKey key = Microsoft.Win32.Registry.LocalMachine.CreateSubKey(@"SOFTWARE\MapInfo\MapXtreme\6.6");
			string s = (string)key.GetValue("SampleDataSearchPath");
			if (s != null && s.Length > 0) 
			{
				if (s.EndsWith("\\")==false) 
				{
					s += "\\";
				}
			}
			else 
			{
				s = Environment.CurrentDirectory;
			}
			key.Close();
	
			Session.Current.TableSearchPath.Path = s;		

			// Listen to the layers added event so that we can refresh the 
			// 
			mapControl1.Map.Layers.Added +=new CollectionEventHandler(Layers_Added);
			mapControl1.Map.Layers.Removed += new CollectionEventHandler(Layers_Removed);
			layers.SelectedValueChanged +=new EventHandler(layers_SelectedValueChanged);
		}

		private void Layers_Added(object sender, CollectionEventArgs e)
		{			
			layers.Items.Insert(e.Position, e.Item);
			if(layers.Items.Count == 1)
			{
				layers.SelectedItem = e.Item;
			}
		}

		private void Layers_Removed(object sender, CollectionEventArgs e)
		{
			if(e.Item == layers.SelectedItem) 
			{
				fields.Items.Clear();
				DeletePieTheme();

				layers.SelectedIndex = (layers.SelectedIndex > 0)? layers.SelectedIndex - 1 : (layers.Items.Count > 1)? 1 : -1;
			}
			layers.Items.Remove(e.Item);
		}

		private void layers_SelectedValueChanged(object sender, EventArgs e)
		{
			fields.Items.Clear();
			DeletePieTheme();

			FeatureLayer selected = layers.SelectedItem as FeatureLayer;
			if(selected != null)
			{
				foreach(MapInfo.Data.Column column in selected.Table.TableInfo.Columns)
				{
					if(column.Alias != "Obj" && column.Alias != "MI_Style")
					{
						fields.Items.Add(column.Alias, CheckState.Unchecked);
					}
				}
			}
		}

		private void fields_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			DeletePieTheme();
			AddPieTheme();
		}

		private void DeletePieTheme()
		{
			// Remove previous themes:
			MapLayerEnumerator e = mapControl1.Map.Layers.GetMapLayerEnumerator(MapLayerFilterFactory.FilterByType(typeof(ObjectThemeLayer)));
			while(e.MoveNext())
			{
				mapControl1.Map.Layers.Remove(e.Current);
				e = mapControl1.Map.Layers.GetMapLayerEnumerator(MapLayerFilterFactory.FilterByType(typeof(ObjectThemeLayer)));
			}
		}

		private void AddPieTheme()
		{
			FeatureLayer selected = layers.SelectedItem as FeatureLayer;
			if(selected != null)
			{
				// Collect list of columns to use:
				ArrayList list = new ArrayList();
				for(int i=0; i < fields.Items.Count; i++)
				{
					if(fields.GetItemCheckState(i) == CheckState.Checked)
					{
						list.Add(fields.Items[i]);
					}
				}

				// If there are any, then add the theme:
				if(list.Count > 0)
				{
					try 
					{
						PieTheme theme = new PieTheme(
							mapControl1.Map,
							selected.Table,
							(string[])list.ToArray(typeof(string))
							);
						ObjectThemeLayer themeLayer = new ObjectThemeLayer(
							"World Pop Growth Rate", null, theme);				
								
						mapControl1.Map.Layers.Add(themeLayer);
						theme.DataValueAtSize *= 2;
						theme.GraduateSizeBy = GraduateSizeBy.Constant;
						themeLayer.RebuildTheme();
					}
					catch (Exception e) 
					{
						for(int i=0; i < fields.Items.Count; i++)
						{
							fields.SetItemCheckState(i,CheckState.Unchecked);
						}
						System.Windows.Forms.MessageBox.Show("Error: \n"+e.ToString(),"PieTheme Error");
					}
				}
			}	  
		}
	}
}
