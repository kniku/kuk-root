import { Injectable, Inject } from '@angular/core';
import { Observable, of } from 'rxjs';
import { catchError, tap } from 'rxjs/operators';
import { HttpClient } from '@angular/common/http';
import { Konto } from "../model/konto"
import { environment } from '../../environments/environment';

@Injectable({
  providedIn: 'root'
})
export class KontoService {

  constructor(private http: HttpClient) { }

  public Get(id?: number): Array<Konto>
  {
    return new Array<Konto>();
  }

  public getKonten(): Observable<Konto[]>{
    return this.http.get<Konto[]>('v1/accounts')
      .pipe(
        tap(heroes => this.log(`fetched konten`)),
        catchError(this.handleError('getKonten', []))
      );
  }

    /**
   * Handle Http operation that failed.
   * Let the app continue.
   * @param operation - name of the operation that failed
   * @param result - optional value to return as the observable result
   */
  private handleError<T> (operation = 'operation', result?: T) {
    return (error: any): Observable<T> => {
 
      // TODO: send the error to remote logging infrastructure
      console.error(error); // log to console instead
 
      // TODO: better job of transforming error for user consumption
      this.log(`${operation} failed: ${error.message}`);
 
      // Let the app keep running by returning an empty result.
      return of(result as T);
    };
  }

  private log(message: string) {
    console.log('KontoService: ' + message);
  }

}
