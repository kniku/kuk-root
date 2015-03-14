using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using System.Windows.Forms;
using System.Drawing;

namespace KLib.Forms
{
	public class CFormsDataViewer
	{
		DataGridView GridData;

		public CFormsDataViewer()
		{
			GridData = new DataGridView();
			GridData.Font = new System.Drawing.Font("Courier New", 8f, FontStyle.Regular);
			GridData.SelectionMode = DataGridViewSelectionMode.CellSelect;
			GridData.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells;
			GridData.RowHeadersVisible = false;
			GridData.ReadOnly = true;
			GridData.AllowUserToResizeRows = false;
			GridData.AllowUserToResizeColumns = true;
			GridData.AllowUserToAddRows = false;
		}

		public void appendData(object Data)
		{
			if (Data != null)
			{
				FieldInfo[] fields = Data.GetType().GetFields();

				if (GridData.ColumnCount == 0)
				{
					GridData.ColumnCount = fields.Count();
					int idx = 0;
					foreach (var field in fields)
					{
						GridData.Columns[idx++].Name = field.Name;
					}
				}

				DataGridViewRow aRow = new DataGridViewRow();
				aRow.Height = 16;

				foreach (var field in fields)
				{
					DataGridViewTextBoxCell aCell;

					aCell = new DataGridViewTextBoxCell();
					aCell.Value = field.GetValue(Data);
					aRow.Cells.Add(aCell);
				}

				GridData.Rows.Add(aRow);
			}
		}

		public Form showData(Form Parent, string Title)
		{
			Form Dlg = new Form();
			Dlg.Text = Title;
			if (Parent != null)
			{
				Dlg.Icon = Parent.Icon;
			}

			Dlg.StartPosition = FormStartPosition.WindowsDefaultLocation;
			Dlg.Height = 600;
			Dlg.Width = 400;

			GridData.Parent = Dlg;
			GridData.SetBounds(Dlg.Bounds.X, Dlg.Bounds.Y + 4, Dlg.Bounds.Width - 7, Dlg.Bounds.Height - 60);
			GridData.Anchor = AnchorStyles.Left | AnchorStyles.Right | AnchorStyles.Top | AnchorStyles.Bottom;

			Button But = new Button();
			But.Text = "Abbrechen";
			But.Parent = Dlg;
			But.SetBounds(Dlg.Bounds.X + (Dlg.Bounds.Width - 200) / 2, Dlg.Bounds.Height - 53, 200, 25);
			But.Anchor = AnchorStyles.Left | AnchorStyles.Bottom;
			But.FlatStyle = FlatStyle.Flat;
			But.Click += new EventHandler(delegate { Dlg.Close(); });
			Dlg.CancelButton = But;

			Dlg.Show(Parent);

			return Dlg;
		}
	}
}
