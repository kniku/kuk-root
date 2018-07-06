import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { BankauszuegeComponent } from './bankauszuege.component';

describe('BankauszuegeComponent', () => {
  let component: BankauszuegeComponent;
  let fixture: ComponentFixture<BankauszuegeComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ BankauszuegeComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(BankauszuegeComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
