import { Injectable } from '@angular/core';
import { Konto } from "../model/konto"

@Injectable({
  providedIn: 'root'
})
export class KontoService {

  constructor() { }

  public Get(id?: number): Array<Konto>
  {
    return new Array<Konto>();
  }
}
