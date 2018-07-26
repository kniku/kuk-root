import { BrowserModule } from '@angular/platform-browser';
import { NgModule, Injectable } from '@angular/core';
import { HttpClientModule, HTTP_INTERCEPTORS } from '@angular/common/http';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { KontenComponent } from './components/konten/konten.component';
import { BankauszuegeComponent } from './components/bankauszuege/bankauszuege.component';
import { TestComponent } from './components/test/test.component';
import { BaseUrlInterceptor } from './common/http/BaseUrlInterceptor';

@NgModule({
  declarations: [
    AppComponent,
    KontenComponent,
    BankauszuegeComponent,
    TestComponent
  ],
  imports: [
    BrowserModule,
    HttpClientModule,
    AppRoutingModule
  ],
  providers: [
    { provide: HTTP_INTERCEPTORS, useClass: BaseUrlInterceptor, multi: true }
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
