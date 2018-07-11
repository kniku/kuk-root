import { Component, OnInit } from '@angular/core';
import { KontoService } from '../../service/konto.service';
import { Konto } from '../../model/konto';

@Component({
  selector: 'app-konten',
  templateUrl: './konten.component.html',
  styleUrls: ['./konten.component.css']
})
export class KontenComponent implements OnInit {
  Konten: Konto[];
  selectedIndex: number = -1;

  constructor(private kontoService: KontoService) { }

  ngOnInit() {
    this.kontoService.getKonten().subscribe(k => {
      this.Konten = k;
      this.selectedIndex = 0;
    });
  }
}
