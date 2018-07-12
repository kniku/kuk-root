import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { KontenComponent } from './components/konten/konten.component';
import { BankauszuegeComponent } from './components/bankauszuege/bankauszuege.component';
import { TestComponent } from './components/test/test.component';

const routes: Routes = [
  { path: 'konten', component: KontenComponent },
  { path: 'bankauszuege', component: BankauszuegeComponent },
  { path: 'test', component: TestComponent },
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
