import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { HttpClientModule } from '@angular/common/http';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { KontenComponent } from './components/konten/konten.component';
import { BankauszuegeComponent } from './components/bankauszuege/bankauszuege.component';

@NgModule({
  declarations: [
    AppComponent,
    KontenComponent,
    BankauszuegeComponent
  ],
  imports: [
    BrowserModule,
    HttpClientModule,
    AppRoutingModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
