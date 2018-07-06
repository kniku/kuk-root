import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { KontenComponent } from './components/konten/konten.component';
import { BankauszuegeComponent } from './components/bankauszuege/bankauszuege.component';

const routes: Routes = [
  { path: 'konten', component: KontenComponent },
  { path: 'bankauszuege', component: BankauszuegeComponent },
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
